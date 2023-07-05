1. Inicie o DFS a partir do nó inicial.
2. Marque o nó atual como visitado.
3. Verifique se o nó atual é o objetivo. Se for, retorne o caminho até ele.
4. Explore os nós vizinhos não visitados do nó atual em ordem.
5. Para cada nó vizinho não visitado, faça o seguinte:
   1. Chame recursivamente o DFS para o nó vizinho
   2. Marque o nó vizinho como visitado.
   3. Atualize o caminho percorrido, se necessário.
6. Após explorar todos os vizinhos, retroceda para o nó anterior.
7. Repita os passos 4 a 6 até explorar todos os caminhos possíveis.
8. Se não houver mais nós para explorar ou alcançar o objetivo, retorne sem solução.

1. X e Y =0
2. Descobre filhos
3. 
