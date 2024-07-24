# Análise Estrutural Plana - AEP Solver

O AEP Solver é um software desenvolvido para resolver estruturas de barra utilizando o método dos elementos finitos com base na teoria da viga de Euler-Bernoulli. A formulação se baseia na equação diferencial da viga de Euler-Bernoulli:
A formulação do problema se baseia na seguinte equação diferencial, que descreve o comportamento de uma viga sujeita a flexão:

$$
\frac{d^2}{dx^2}\left(EI\frac{d^2u}{dx^2}\right) = q(x)
$$

onde $E$ é o módulo de elasticidade do material, $I$ é o momento de inércia da seção transversal, $u(x)$ é a deflexão da viga na posição $x$ e $q(x)$ é a carga distribuída ao longo da viga.


## Como Usar

Para utilizar o programa, baixe o arquivo executável disponibilizado na aba [Releases](link-para-releases) do GitHub. Extraia o arquivo zipado em seu computador e execute o aplicativo.

## Screenshots

Veja abaixo algumas imagens do AEP Solver em ação:

![Screenshot 1](screenshots/ss1.png)

![Screenshot 2](screenshots/ss2.png)

![Screenshot 3](screenshots/ss3.png)

![Screenshot 4](screenshots/ss4.png)

## Dependências

O software utiliza as seguintes bibliotecas:
- [Raylib](https://www.raylib.com/)
- [ImGui](https://github.com/ocornut/imgui)
- [Eigen](http://eigen.tuxfamily.org/index.php?title=Main_Page)

## Compilação

Para compilar o código-fonte, é necessário ter instalada a biblioteca Raylib. Recomenda-se o uso de [Visual Studio Code](https://code.visualstudio.com/) para facilitar a compilação e configuração do projeto.

## Contato

Se você tiver qualquer dúvida ou sugestão, não hesite em entrar em contato:

- **Eduardo Vicente Wolf Trentini**
- Email: [eduardowtrentini@gmail.com](mailto:eduardowtrentini@gmail.com)

