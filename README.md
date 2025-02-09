# TAREFA: Interfaces de comunicação Serial com RP2040

Essa tarefa foi proposta para consolidar os conceitos sobre o uso de interfaces de comunicação serial no RP2040 e explorar as funcionalidades da placa de Desenvolvimento BitDogLab.

---

## 📋 DESCRIÇÃO

Neste projeto deve ser feito o uso de comunicação serial UART e I2C, para interações com o monitor serial e display Oled, utilizar interrupções e botões com tratamento de debounce. Deverar ser utilizado os seguintes componentes conectados na placa de desenvolvimento BitDogLab:

    - Matriz 5x5 de LEDs (endereçáveis) WS2812, conectada à GPIO 7. 
    - LED RGB, com os pinos conectados às GPIOs (11, 12 e 13). 
    - Botão A conectado à GPIO 5. 
    - Botão B conectado à GPIO 6. 
    - Display SSD1306 conectado via I2C (GPIO 14 e GPIO15).

---

## OBJETIVOS

    - Compreender o funcionamento e aplicações de comunicação Serial em microcontroladores;
    - Aplicar os conhecimentos adquiridos nas aulas síncronas e assíncronas sobre UART e I2C na prática;
    - Manipular e controlar o LED RGB, a matriz de LEDs endereçáveis WS2812 e o Display Oled;
    - Fixar o estudo sobre botões de acionamento, interrupções e Debounce;
    - Desenvolver um projeto Funcional que combine Hardware e Software.

---

## ✨ FUNCIONALIDADES DO PROJETO

    **1. Fazer modificação no arquivo font.h da biblioteca inc:**
        - Essa alteração consiste em adicionar novos caracteres, por exemplo, caracteres minúsculos.
    
    **2. Entrada de caracteres via PC:**  
        - Utilize o Serial Monitor do VS Code para digitar os caracteres. 
        
        - Cada caractere digitado no Serial Monitor deve ser exibido no display SSD1306. 
        Observação: Apenas um caractere será enviado de cada vez, não é necessário suportar o 
        envio de strings completas. 
        
        - Quando um número entre 0 e 9 for digitado, um símbolo correspondente ao número deve 
        ser exibido, também, na matriz 5x5 WS2812.  
        
    **3. Interação com o Botão A:**  
        - Pressionar o botão A deve alternar o estado do LED RGB Verde (ligado/desligado). 
        - A operação deve ser registrada de duas formas: 
            - Uma mensagem informativa sobre o estado do LED deve ser exibida no display SSD1306 
            - Um texto descritivo sobre a operação deve ser enviado ao Serial Monitor. 

    **4. Interação com o Botão B:** 
        - Pressionar o botão B deve alternar o estado do LED RGB Azul (ligado/desligado). 
        - A operação deve ser registrada de duas formas: 
            - Uma mensagem informativa sobre o estado do LED deve ser exibida no display SSD1306 
            - Um texto descritivo sobre a operação deve ser enviado ao Serial Monitor. 
 
---

## 🎥 VIDEO DE DEMONSTRAÇÃO

Confira o funcionamento do projeto nos seguintes links:

Coloquei 2 links por que a qualidade não ficou muito bom. O primeiro link é melhor para ver a parte da placa
O segundo video é melhor para ver a parte do código

[📹 Ver vídeo de Demonstração](https://drive.google.com/file/d/1jfYUDZAdWRXTTkCCIdxY2aG6xMd-aCaR/view?usp=sharing)
[📹 Ver vídeo de Demonstração](https://drive.google.com/file/d/1_8SCgNTDLc_d-6FjYdJWILwOOsL3Bose/view?usp=sharing)
