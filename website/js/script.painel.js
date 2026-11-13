"use strict";
// Funcionalidades adicionais da página Serviços.
// Ajuste os caminhos após confirmar os aliases dos executáveis CGI com a equipe do Apache.
const ROTAS_PAINEL = {
  registrarAtendimento: "/cgi-bin/registrar_atendimento",
  registrarServico: "/cgi-bin/registrar_servico",
  historico: "/cgi-bin/historico_atendimentos",
  relatorio: "/cgi-bin/relatorio_atendimento",
  faturamento: "/cgi-bin/relatorio_faturamento",
  listarServicos: "/cgi-bin/listar_servicos"
};
const $ = id => document.getElementById(id);
const texto = (id, valor) => { $(id).textContent = valor; };
const real = valor => new Intl.NumberFormat("pt-BR", {style:"currency",currency:"BRL"}).format(Number(valor)||0);
function dataParaC(iso) {
  const partes = /^(\d{4})-(\d{2})-(\d{2})$/.exec(iso);
  if (!partes) throw new Error("Informe uma data válida.");
  return `${partes[3]}/${partes[2]}/${partes[1]}`;
}
async function solicitar(url, opcoes) {
  const resposta = await fetch(url, opcoes);
  const tipo = resposta.headers.get("content-type") || "";
  if (!tipo.includes("application/json")) throw new Error(`O servidor não devolveu JSON (HTTP ${resposta.status}). Verifique a rota CGI.`);
  const json = await resposta.json();
  if (!resposta.ok || json.sucesso !== true) throw new Error(json.mensagem || `Erro HTTP ${resposta.status}`);
  return json.dados;
}
function enviar(url, dados) {
  return solicitar(url, {method:"POST", headers:{"Content-Type":"application/json"},body:JSON.stringify(dados)});
}
function criarTabela(id, registros, campos) {
  const corpo = $(id);
  corpo.replaceChildren();
  for (const registro of registros) {
    const tr = document.createElement("tr");
    for (const campo of campos) {
      const td = document.createElement("td");
      td.textContent = String(campo(registro) ?? "");
      tr.appendChild(td);
    }
    corpo.appendChild(tr);
  }
}
async function atualizarSeletores() {
  const animais = await solicitar(API_ANIMAIS);
  if (!Array.isArray(animais)) throw new Error("Resposta de animais inválida.");
  for (const id of ["at-animal","sv-animal","hist-animal"]) {
    const select = $(id);
    const anterior = select.value;
    select.replaceChildren(new Option("Selecione um animal", ""));
    for (const a of animais) select.add(new Option(`${a.id} — ${a.nome}`, String(a.id)));
    select.value = anterior;
  }
}
async function abrirTelaAdicional(id) {
  try {
    if (["tela-atendimento","tela-servico","tela-historico"].includes(id)) await atualizarSeletores();
    if (id === "tela-relatorio") await carregarRelatorio();
    if (id === "tela-faturamento") await carregarFaturamento();
  } catch (erro) {
    const destino = ({"tela-atendimento":"mensagem-atendimento","tela-servico":"mensagem-servico","tela-historico":"mensagem-historico","tela-relatorio":"mensagem-relatorio","tela-faturamento":"mensagem-faturamento"})[id];
    if (destino) texto(destino, `Não foi possível carregar: ${erro.message}`);
  }
}
// O script.servicos.js existente faz a troca de telas; este script carrega os dados ao abrir.
document.querySelectorAll('.painel-botao').forEach(botao => {
  botao.addEventListener('click', () => abrirTelaAdicional(botao.dataset.tela));
});
$('form-atendimento').addEventListener('submit', async evento => {
  evento.preventDefault();
  texto('mensagem-atendimento','Enviando atendimento...');
  try {
    const dados={idAnimal:Number($('at-animal').value),data:dataParaC($('at-data').value),descricao:$('at-descricao').value.trim()};
    await enviar(ROTAS_PAINEL.registrarAtendimento,dados);
    texto('mensagem-atendimento','Atendimento registrado com sucesso.');
    evento.target.reset();
  } catch (erro) { texto('mensagem-atendimento',`Erro: ${erro.message}`); }
});
$('form-servico').addEventListener('submit', async evento => {
  evento.preventDefault();
  texto('mensagem-servico','Enviando serviço...');
  try {
    const dados={idAnimal:Number($('sv-animal').value),data:dataParaC($('sv-data').value),descricao:$('sv-descricao').value.trim(),valor:Number($('sv-valor').value)};
    await enviar(ROTAS_PAINEL.registrarServico,dados);
    texto('mensagem-servico','Serviço registrado com sucesso.');
    evento.target.reset();
  } catch (erro) { texto('mensagem-servico',`Erro: ${erro.message}`); }
});
async function carregarHistorico() {
  const id=$('hist-animal').value;
  if (!id) { texto('mensagem-historico','Selecione um animal.'); return; }
  texto('mensagem-historico','Consultando histórico...');
  try {
    const dados=await solicitar(`${ROTAS_PAINEL.historico}?id=${encodeURIComponent(id)}`);
    if (!Array.isArray(dados)) throw new Error('Formato do histórico inválido.');
    criarTabela('corpo-historico',dados,[a=>a.idAnimal,a=>a.data,a=>a.descricao]);
    texto('mensagem-historico',`${dados.length} atendimento(s) encontrado(s).`);
  } catch(erro) { $('corpo-historico').replaceChildren();texto('mensagem-historico',`Erro: ${erro.message}`); }
}
$('consultar-historico').addEventListener('click',carregarHistorico);
async function carregarRelatorio() {
  texto('mensagem-relatorio','Carregando relatório...');
  try {
    const dados=await solicitar(ROTAS_PAINEL.relatorio);
    if (!Array.isArray(dados)) throw new Error('Formato do relatório inválido.');
    criarTabela('corpo-relatorio',dados,[a=>a.idAnimal,a=>a.data,a=>a.descricao]);
    texto('mensagem-relatorio',`${dados.length} atendimento(s) no relatório.`);
  } catch(erro) { $('corpo-relatorio').replaceChildren();texto('mensagem-relatorio',`Erro: ${erro.message}`); }
}
$('atualizar-relatorio').addEventListener('click',carregarRelatorio);
async function carregarFaturamento() {
  texto('mensagem-faturamento','Carregando faturamento...');
  try {
    const [resumo,servicos]=await Promise.all([solicitar(ROTAS_PAINEL.faturamento),solicitar(ROTAS_PAINEL.listarServicos)]);
    if (!resumo || !Array.isArray(servicos)) throw new Error('Formato do faturamento inválido.');
    texto('fat-quantidade',String(resumo.quantidadeServicos ?? '—'));
    texto('fat-total',real(resumo.faturamentoTotal));
    criarTabela('corpo-servicos',servicos,[s=>s.id,s=>s.idAnimal,s=>s.data,s=>s.descricao,s=>real(s.valor)]);
    texto('mensagem-faturamento',`${servicos.length} serviço(s) na listagem.`);
  } catch(erro) {
    $('corpo-servicos').replaceChildren();texto('fat-quantidade','—');texto('fat-total','—');
    texto('mensagem-faturamento',`Erro: ${erro.message}`);
  }
}
$('atualizar-faturamento').addEventListener('click',carregarFaturamento);
