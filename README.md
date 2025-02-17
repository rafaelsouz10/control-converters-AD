# Sistema de controle de Conversores A/D

---

## Descrição do Projeto

Projeto da atividade **EmbarcaTech**, que visa implementar o controle de um quadrado móvel dentro de diferentes retângulos em um display OLED SSD1306, utilizando um joystick analógico para movimentação, alterando a intensidade do led via pwm com o joystick e utilizando o botão para ativar/desativar os leds pwm. O botão do joystick delimita o retângulo que visa a area de liberdade do quadrado.

![image](https://github.com/user-attachments/assets/2b84b6d7-3261-45ee-b7d3-58058c3c51a5)


---

## Requisitos

- **Microcontrolador**: Raspberry Pi Pico ou Raspberry Pi Pico W (opcional)
- **Placa de Desenvolvimento:** BitDogLab (opcional).
- **Conta Criada no Wokwi Simulator**: [Wokwi](https://wokwi.com/).
- **Editor de Código**: Visual Studio Code (VS Code).
- **SDK do Raspberry Pi Pico** configurado no sistema.
- Ferramentas de build: **CMake** e **Ninja**.

---

## Instruções de Uso

### 1. Clone o Repositório

Clone o repositório para o seu computador:
```bash
git clone https://github.com/rafaelsouz10/control-converters-AD.git
cd control-converters-AD
code .
```
---

### 2. Instale as Dependências

Certifique-se de que o SDK do Raspberry Pi Pico está configurado corretamente no VS Code. As extensões recomendadas são:

- **C/C++** (Microsoft).
- **CMake Tools**.
- **Wokwi Simulator**.
- **Raspberry Pi Pico**.

---

### 3. Configure o VS Code

Abra o projeto no Visual Studio Code e siga as etapas abaixo:

1. Certifique-se de que as extensões mencionadas anteriormente estão instaladas.
2. No terminal do VS Code, compile o código clicando em "Compile Project" na interface da extensão do Raspberry Pi Pico.
3. O processo gerará o arquivo `.uf2` necessário para a execução no hardware real.

---

### 4. Teste no Simulador Wokwi Integrado ao VS Code

Após ter configurado o VS Code conforme descrito no item 3, siga os passos abaixo para simular o projeto:

1. Abra o arquivo `diagram.json` no Visual Studio Code.
2. Clique no botão "Play" disponível na interface.
3. Divirta-se interagindo com os componentes disponíveis no simulador integrado!

---

### 5. Teste no Hardware Real

#### Utilizando a Placa de Desenvolvimento BitDogLab com Raspberry Pi Pico W:

1. Conecte a placa ao computador no modo BOTSEEL:
   - Pressione o botão **BOOTSEL** (localizado na parte de trás da placa de desenvolvimento) e, em seguida, o botão **RESET** (localizado na frente da placa).
   - Após alguns segundos, solte o botão **RESET** e, logo depois, solte o botão **BOOTSEL**.
   - A placa entrará no modo de programação.

2. Compile o projeto no VS Code utilizando a extensão do [Raspberry Pi Pico W](https://marketplace.visualstudio.com/items?itemName=raspberry-pi.raspberry-pi-pico):
   - Clique em **Compile Project**.

3. Execute o projeto clicando em **Run Project USB**, localizado abaixo do botão **Compile Project**.

---

### 🔌 6. Conexões e Esquemático
Abaixo está o mapeamento de conexões entre os componentes e a Raspberry Pi Pico W:

| **Componentes**        | **Pino Conectado (GPIO)** |
|------------------------|---------------------------|
| Display SSD1306 (SDA)  | GPIO 14                   |
| Display SSD1306 (SCL)  | GPIO 15                   |
| Joystick VRX           | GPIO 27                   |
| Joystick VRY           | GPIO 26                   |
| Botão A                | GPIO 5                    |
| LED RGB Vermelho       | GPIO 13                   |
| LED RGB Azul           | GPIO 12                   |
| LED RGB Verde          | GPIO 11                   |

#### 🛠️ Hardware Utilizado
- **Microcontrolador:** Raspberry Pi Pico W
- **Display:** OLED SSD1306 (I2C)
- **Joystick Analógico** (VRX e VRY)
- **Botão para alternar entre os estágios**
- **Led RGB**
---

### 7. Funcionamento do Sistema

#### 📌 Funcionalidades
- Exibição de até **quatro estágios** de retângulos no display:
  - **Estágio 0:** Sem retângulo (área livre de 128x64 pixels).
  - **Estágio 1:** Retângulo grande (128x64 pixels).
  - **Estágio 2:** Retângulo médio (108x54 pixels, centralizado).
  - **Estágio 3:** Retângulo menor (88x44 pixels, centralizado).
- Movimento do quadrado (8x8 pixels) dentro dos limites do retângulo atual.
- Troca de estágio dos retângulos ao pressionar um botão.
- Controle do movimento via **joystick analógico**.
- Atualização da tela utilizando a biblioteca **ssd1306**.
- Movimentação do **eixo y** do Joystick do centro para cima muda intensidade do led azul.
- Movimentação do **eixo x** do Joystick do centro para direita muda a intensidade do led vermelho.
- **Botão do joystick** alterna o estado do led verde.
- O **botão A** ativa/desativa os leds pwm e também o led verde (opção a mais colocada).

--- 

### 8. Vídeo Demonstrativo

Click [AQUI](O lindo do vídeo irá aqui) para acessar o link do Vídeo Ensaio
