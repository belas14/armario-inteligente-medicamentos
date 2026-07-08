
// ARMARIO INTELIGENTE DE MEDICAMENTOS

// LEDs
const int ledVermelho = 11;
const int ledLaranja = 10;
const int ledAmarelo = 9;
const int ledVerde = 6;
const int ledAzul = 5;

// Buzzer
const int buzzer = 3;

// Botao
const int botaoConfirmar = 4;

// Potenciometro ligado no A2
const int potenciometro = A2;

// Controle do sistema
int medicamentoAtual = 0;
int pontos = 0;
bool alertaAtivo = false;

unsigned long tempoInicioAlerta = 0;
const unsigned long tempoLimite = 15000; // 15 segundos para confirmar

// Filtro de media movel
const int tamanhoFiltro = 5;
int leituras[tamanhoFiltro];
int indice = 0;
long soma = 0;

// Rotina de teste do sistema
void rotinaTeste()
{
  Serial.println("=== INICIANDO ROTINA DE TESTE ===");

  apagarLeds();

  digitalWrite(ledVermelho, HIGH);
  delay(400);
  digitalWrite(ledVermelho, LOW);

  digitalWrite(ledLaranja, HIGH);
  delay(400);
  digitalWrite(ledLaranja, LOW);

  digitalWrite(ledAmarelo, HIGH);
  delay(400);
  digitalWrite(ledAmarelo, LOW);

  digitalWrite(ledVerde, HIGH);
  delay(400);
  digitalWrite(ledVerde, LOW);

  digitalWrite(ledAzul, HIGH);
  delay(400);
  digitalWrite(ledAzul, LOW);

  Serial.println("Teste do buzzer...");
  tone(buzzer, 1000);
  delay(700);
  noTone(buzzer);

  Serial.println("Teste concluido com sucesso!");
  Serial.println("Agora sera iniciado um alerta de medicamento.");

  chamarMedicamento(1);
}

void setup()
{
pinMode(ledVermelho, OUTPUT);
pinMode(ledLaranja, OUTPUT);
pinMode(ledAmarelo, OUTPUT);
pinMode(ledVerde, OUTPUT);
pinMode(ledAzul, OUTPUT);

pinMode(buzzer, OUTPUT);
pinMode(botaoConfirmar, INPUT_PULLUP);

Serial.begin(9600);

for (int i = 0; i < tamanhoFiltro; i++)
{
leituras[i] = 0;
}

apagarLeds();

Serial.println("ARMARIO INTELIGENTE DE MEDICAMENTOS");
Serial.println("Comandos:");
Serial.println("1 - Medicamento da manha");
Serial.println("2 - Medicamento da tarde");
Serial.println("3 - Medicamento da noite");
Serial.println("P - Mostrar pontuacao");
Serial.println("T - Rotina de teste");
}

void loop()
{
lerBluetooth();
verificarBotao();

if (alertaAtivo)
{
tocarAlerta();
verificarAtraso();
}
}

// Le os comandos enviados pelo celular ou Monitor Serial
void lerBluetooth()
{
if (Serial.available() > 0)
{
char comando = Serial.read();

if (comando == '1')
{
chamarMedicamento(1);
}
else if (comando == '2')
{
chamarMedicamento(2);
}
else if (comando == '3')
{
chamarMedicamento(3);
}
else if (comando == 'P' || comando == 'p')
{
mostrarPontuacao();
}
else if (comando == 'T' || comando == 't')
{
    rotinaTeste();
}
}
}

// Ativa alerta do medicamento
void chamarMedicamento(int medicamento)
{
medicamentoAtual = medicamento;
alertaAtivo = true;
tempoInicioAlerta = millis();

apagarLeds();

if (medicamento == 1)
{
digitalWrite(ledAzul, HIGH);
Serial.println("Medicamento da MANHA acionado.");
}
else if (medicamento == 2)
{
digitalWrite(ledAmarelo, HIGH);
Serial.println("Medicamento da TARDE acionado.");
}
else if (medicamento == 3)
{
digitalWrite(ledLaranja, HIGH);
Serial.println("Medicamento da NOITE acionado.");
}

Serial.println("Aperte o botao para confirmar que tomou.");
}

// Verifica se o usuario apertou o botao
void verificarBotao()
{
if (alertaAtivo && digitalRead(botaoConfirmar) == LOW)
{
delay(200);

alertaAtivo = false;
noTone(buzzer);
apagarLeds();

digitalWrite(ledVerde, HIGH);

pontos = pontos + 10;

Serial.print("Medicamento ");
Serial.print(medicamentoAtual);
Serial.println(" confirmado!");

Serial.print("Pontuacao atual: ");
Serial.println(pontos);

delay(1500);
digitalWrite(ledVerde, LOW);
}
}

// Se passar do tempo limite, marca como atrasado
void verificarAtraso()
{
if (millis() - tempoInicioAlerta > tempoLimite)
{
apagarLeds();
digitalWrite(ledVermelho, HIGH);

Serial.println("Medicamento atrasado!");
Serial.println("Voce perdeu 5 pontos.");

pontos = pontos - 5;

alertaAtivo = false;
noTone(buzzer);

delay(2000);
digitalWrite(ledVermelho, LOW);
}
}

// Toca alerta usando o potenciometro como ajuste de sensibilidade
void tocarAlerta()
{
int valorFiltrado = lerPotenciometroFiltrado();

int frequencia = map(valorFiltrado, 0, 1023, 300, 2000);

tone(buzzer, frequencia);
delay(150);
noTone(buzzer);
delay(150);
}

// Filtro de media movel no sensor analogico
int lerPotenciometroFiltrado()
{
soma = soma - leituras[indice];

leituras[indice] = analogRead(potenciometro);

soma = soma + leituras[indice];

indice++;

if (indice >= tamanhoFiltro)
{
indice = 0;
}

return soma / tamanhoFiltro;
}

// Apaga todos os LEDs
void apagarLeds()
{
digitalWrite(ledVermelho, LOW);
digitalWrite(ledLaranja, LOW);
digitalWrite(ledAmarelo, LOW);
digitalWrite(ledVerde, LOW);
digitalWrite(ledAzul, LOW);
}

// Mostra a pontuacao
void mostrarPontuacao()
{
Serial.print("Pontuacao atual: ");
Serial.println(pontos);
}