
# PROJETO ROBÓTICA

---

### Integrantes do Grupo

| Nome                           | RA           |
|--------------------------------|--------------|
| Guilherme Couto Gomes          | 22.122.035-3 |
| Pedro Henrique Algodoal Pinto  | 22.122.072-6 |
| Samir Oliveira da Costa  |              |

---

### Descrição do Projeto

Este projeto consiste em programar um robô no simulador Webots capaz de identificar, entre várias caixas, qual delas é a caixa leve. O critério de identificação é que a caixa leve é a única que se move quando o robô encosta nela. O robô deve navegar pelo ambiente, desviar de obstáculos e, ao encontrar a caixa leve, parar de procurar e girar sobre o próprio eixo.

[![Vídeo de Demonstração](https://img.youtube.com/vi/-zy6p42y844/0.jpg)](https://youtu.be/-zy6p42y844)

---

### Funcionamento do Robô

- O robô utiliza sensores de proximidade para desviar de obstáculos (outras caixas).
- Ele percorre o ambiente até encostar em uma caixa.
- O código monitora a posição de todas as caixas a cada passo da simulação.
- Se alguma caixa se mover após o contato, ela é identificada como a caixa leve.
- Ao identificar a caixa leve, o robô para de procurar e começa a girar sobre o próprio eixo.

---

### Codigo da parte da detecção de movimento da caixa leve

```c

for (int c = 0; c < QtddCaixa; c++) {
  if (caixas[c] != NULL) {
    const double *pos_atual = wb_supervisor_node_get_position(caixas[c]);
    double deltaX = fabs(pos_atual[0] - posicaoAnterior[c][0]);
    double deltaY = fabs(pos_atual[1] - posicaoAnterior[c][1]);
    double deltaZ = fabs(pos_atual[2] - posicaoAnterior[c][2]);
    if (deltaX > LIMIAR_MOVIMENTO || deltaY > LIMIAR_MOVIMENTO || deltaZ > LIMIAR_MOVIMENTO) {
      encontrouCaixaLeve = true;
      printf("Encontrou caixa leve! (CAIXA%d)\n", c + 1);
      break;
    }
  }
}
```
