# 🛠️ Instruções e Explicações sobre os Semáforos "One Shot" e "Periódico" 🛠️

🔴🟡🟢 Semáforo Periódico **Cíclico**
O que é?
O semáforo periódico funciona em loop infinito, alternando entre os estados (cores) em intervalos regulares de tempo.
Ele sempre segue essa linha:

Acende o vermelho → espera 3 segundos
Acende o amarelo → espera 3 segundos
Acende o verde → espera 3 segundos
Repete tudo de novo...

Como funciona o código?

O código usa um loop while(true) para rodar continuamente.
Um temporizador controla os intervalos, definindo quando mudar de estado.
A função time_reached() verifica se chegou a hora de trocar o LED.
A cada troca de LED, a variável estado é atualizada para controlar a sequência.


