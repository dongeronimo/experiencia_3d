Experi�ncias com 3d

O objetivo � eu fazer meus pr�prios filtros de GPU e meu pr�prio volume
renderer, de prefer�ncia que possa ser encaixado na VTK.

Lista de tarefas:
1)B�sico. A infraestrutura pro futuro est� pronta. Tenho onde fazer minhas
opera��es de opengl, tenho meus ganchos nos processos da VTK.
1.1)Carregar imagem usando a minha biblioteca.  FEITO (05/06/2017)
1.2)Criar meu actor onde farei as coisas. COMECEI EM (05/06/2017)
1.2.1)Criar a tela da VTK FEITO EM (06/06/2017)
1.2.2)Por meu actor nela FEITO EM (08/06/2017)
1.2.3)Fazer o actor funcionar FEITO EM (08/06/2017)

1.3)Passar a imagem pra GPU como textura 3d
1.3.1)Fazer aparecer na mem�ria quando ver com o gdebugger
1.3.2)Renderizar em um objeto 3d pra enxerg�-la.

1.4)Fazer um volume renderer de raycast.
1.5)Multiplos objetos na tela
1.6)Intera��o: letras de orienta��o, mouse hover pra pegar os escalares,
carga de fun��o de cor a partir de arquivo.
2)Filtros de GPU.
2.1)Infraestrutura pra usar a opencl
2.2)Sigm�ide
2.3)Liga��o OpenCL-OpenGL
3)Volume Rendering avan�ado.
3.1)Consultar o real-time volume rendering e ver as
putarias dele
4)Filtros de GPU avan�ados.
4.1)Perona-Malik.
4.2)Region Growth.
4.3)Se for possivel Region Growth na GPU, fazer fast marching level set.
4.4)Refazer em Compute Shaders e Cuda.
5)Futuro
5.1)Vers�o Web-Gl
5.2)Vers�o Mobile
5.3)Vers�o Unity3d
