# Bug que estava ocorrendo

Para poder fazer o programar funcionar, não pude utilizar a biblioteca do CC50, pois todas as vezes em que o número selecionado para o tamanho do tabuleiro (variável `d`, obtida pelo `d = atoi(argv[1])`), como o 3, por exemplo, chegava na posição `board[0][1]`, a variável `d` se tornava **-1**. O `d` se tornava **-1** após pressionar ENTER com o 3 digitado para ir para a posição `board[0][1]`. Então troquei o `GetInt()`, funcão para receber um inteiro da biblioteca do **CC50**, para um `scanf()` e o bug não ocorreu mais.
