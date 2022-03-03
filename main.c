#include <gtk/gtk.h> // Bilioteca GTK
#include <stdlib.h> // Biblioteca para trabajar con strings

// funciones iniciales
void initialising(GtkButton *buttonInit,int i,int j);
int hasAnyoneWon(int a[3][3]);
void setAllButtonsToBlank();

// Declaracion de varaibles globales
static GtkButton *statusClick,*gameClick;
static GtkButton *button[3][3]={{NULL,NULL,NULL},{NULL,NULL,NULL},{NULL,NULL,NULL}};
static int flag=0, gameNotOver=1, initialise=0, statusFlag=0, pressed[3][3]={{0,0,0},{0,0,0},{0,0,0}};
static int moveCounter=0;

// Declaracion de parametros del juego
static int arr[3][3]={{0,0,0},{0,0,0},{0,0,0}};
static int gameType=0;

// Se comienza a ejecutar
int main(int argc, char *argv[])
{
    GtkBuilder      *builder;
    GtkWidget       *window;

    gtk_init(&argc, &argv);

// inicializacion GTKbuilder con aechivo.glade
    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "mainUI.glade", NULL);

// inicializacion del widget para ventana principal
    window = GTK_WIDGET (gtk_builder_get_object(builder, "Juego"));
    gtk_builder_connect_signals(builder, NULL);

// puntero de desreferenciacion
    g_object_unref(builder);

// Se muestra la ventana principal
    gtk_widget_show(window);
    gtk_main();
    return 0;
}

// A continuacion se crean las funciones para los botones del menu

//  Funcion para salir del juego
void on_gameExit_activate()
{
    gtk_main_quit();
    exit(0);
}

// Funcion para instrucciones
void on_aboutSelection_activate()
{
  GtkWidget   *about;
  GtkBuilder  *builder;

// inicializacion del GTKbuilder con archivo.glade
  builder = gtk_builder_new();
  gtk_builder_add_from_file (builder, "mainUI.glade", NULL);

// inicializando el widget
  about = GTK_WIDGET(gtk_builder_get_object(builder, "instruccion"));
  gtk_builder_connect_signals(builder, NULL);

// puntero generador de desreferenciacion
  g_object_unref(builder);
  gtk_dialog_run (GTK_DIALOG (about));

// Cuando se activa la se;al de respuesta
  gtk_widget_destroy (about);
}

// Funcion del boton jugar
void on_chooseGamemodeButton_clicked(GtkWidget *click_button, gpointer   user_data)
{
  gameClick = (GtkButton *) user_data;
  statusClick = (GtkButton *) user_data;
  statusFlag=1;
  if(!statusFlag) return;
  if(flag)
  {
    gtk_button_set_label(statusClick, "Reiniciar para nueva partida");
    return;
  }
  flag=1;
  gtk_button_set_label(statusClick, "Jugar");
  GtkBuilder  *builder;
  GtkWidget   *gameDialog;
// inicializacion del GTKbuilder con archivo.glade
  builder = gtk_builder_new();
  gtk_builder_add_from_file (builder, "mainUI.glade", NULL);
  gameDialog = GTK_WIDGET(gtk_builder_get_object(builder, "vent_lis"));
  gtk_builder_connect_signals(builder, NULL);
  g_object_unref(builder);
  gtk_dialog_run (GTK_DIALOG (gameDialog));
// Para cuando el boton de cierra desde la x de la barra de herramientas
  gtk_widget_destroy(gameDialog);
}

// Se ejecuta al seleccionar el boton jugar
void on_listo_clicked(GtkWidget *click_button, gpointer   user_data)
{
  gameType = 0;
  gtk_widget_destroy((GtkWidget *) user_data);
  gtk_button_set_label(gameClick, "Vamos");
}


/* Boton de reinicio del juego, reinicia todas la varaibles globales y
ejecuta el metodo principal */
void on_restartGame_activate(GtkWidget *click_button, gpointer   user_data)
{
  int i,j;
 if(!statusFlag) return;
 gameType=0; flag=0; moveCounter=0; gameNotOver=1; initialise=0;
 GtkWidget *Juego = (GtkWidget *) user_data;
 gtk_widget_destroy(Juego);
  for(i=0;i<3;i++)
  {
      for(j=0;j<3;j++)
      {
          button[i][j]=NULL;
          arr[i][j]=0;
          pressed[i][j]=0;
      }
  }
  main(0, NULL);
  setAllButtonsToBlank();
}


/* A continuacion se procede a programar los botones que se encargan
de mostrar la X o el 0, al moemnto de jugar. */

// Boton en la posicion 11
int on_button11_clicked(GtkWidget *click_button, gpointer   user_data)
{
  GtkButton *buttonTemp = (GtkButton *) user_data;

  // Si esta en modo inicializacion se ejecutar
  if(initialise!=0)
  {
    initialising(buttonTemp,0,0);
    return 0;
  }

  // Se ejecuta si el boton se presiona mientras se esta inicializando el juego
  if(pressed[0][0]) return 0;

  // Se ejecuta cuando se presiona el boton en el juego
  if(flag&&gameNotOver)
  {
    // Se ejecuta durante el movimiento impar
    if(++moveCounter%2)
    {
      arr[0][0]=1;
      gtk_button_set_label(buttonTemp, "X");
      pressed[0][0]=1;
      switch(hasAnyoneWon(arr))
      {
        case 0: if(moveCounter==9)
                {
                  gtk_button_set_label(statusClick, "Empate");
                  gameNotOver=0; return 0;
                } break;
        case 1: gtk_button_set_label(statusClick, "Ganó Jugador 1");
                gameNotOver=0; return 0;
        case 2: gtk_button_set_label(statusClick, "Ganó Jugador 2");
                gameNotOver=0; return 0;
      }
      gtk_button_set_label(statusClick, "Turno del jugador 2");
    }
    // Se ejecuta cuando se presiona el boton durante el movimiento uniforme
    else
    {
      arr[0][0]=2;
      gtk_button_set_label(buttonTemp, "O");
      switch(hasAnyoneWon(arr))
      {
        case 1: gtk_button_set_label(statusClick, "Ganó Jugador 1");
                gameNotOver=0; return 0;
        case 2: gtk_button_set_label(statusClick, "Ganó Jugador 2");
                gameNotOver=0; return 0;
      }
      gtk_button_set_label(statusClick, "Turno del Jugador 1");
    }
    pressed[0][0]=1;
  }
  return 0;
}

// Boton en la posicion 12
int on_button12_clicked(GtkWidget *click_button, gpointer   user_data)
{
  GtkButton *buttonTemp = (GtkButton *) user_data;

  // Si esta en modo inicializacion se ejecuta
  if(initialise!=0)
  {
    initialising(buttonTemp,0,1);
    return 0;
  }

  // Se ejecuta si el boton se presiona mientras se esta inicializando el juego
  if(pressed[0][1]) return 0;

  // Se ejecuta cuando se presiona el boton en el juego
  if(flag&&gameNotOver)
  {
    // Se ejecuta durante el movimiento impar
    if(++moveCounter%2)
    {
      arr[0][1]=1;
      gtk_button_set_label(buttonTemp, "X");
      pressed[0][1]=1;
      switch(hasAnyoneWon(arr))
      {
        case 0: if(moveCounter==9)
                {
                  gtk_button_set_label(statusClick, "Empate");
                  gameNotOver=0; return 0;
                } break;
        case 1: gtk_button_set_label(statusClick, "Ganó Jugador 1");
                gameNotOver=0; return 0;
        case 2: gtk_button_set_label(statusClick, "Ganó Jugador 2");
                gameNotOver=0; return 0;
      }
      gtk_button_set_label(statusClick, "Turno del jugador 2");
    }
    // Se ejecuta cuando se presiona el boton durante el movimiento uniforme
    else
    {
      arr[0][1]=2;
      gtk_button_set_label(buttonTemp, "O");
      switch(hasAnyoneWon(arr))
      {
        case 1: gtk_button_set_label(statusClick, "Ganó Jugador 1");
                gameNotOver=0; return 0;
        case 2: gtk_button_set_label(statusClick, "Ganó Jugador 2");
                gameNotOver=0; return 0;
      }
      gtk_button_set_label(statusClick, "Turno del Jugador 1");
    }
    pressed[0][1]=1;
  }
  return 0;
}

// Boton en la posicion 13
int on_button13_clicked(GtkWidget *click_button, gpointer   user_data)
{
  GtkButton *buttonTemp = (GtkButton *) user_data;

  // Si esta en modo inicializacion se ejecuta
  if(initialise!=0)
  {
    initialising(buttonTemp,0,2);
    return 0;
  }

  // Se ejecuta si el boton se presiona mientras se esta inicializando el juego
  if(pressed[0][2]) return 0;

  // Se ejecuta cuando se presiona el boton en el juego
  if(flag&&gameNotOver)
  {
    // Se ejecuta durante el movimiento impar
    if(++moveCounter%2)
    {
      arr[0][2]=1;
      gtk_button_set_label(buttonTemp, "X");
      pressed[0][2]=1;
      switch(hasAnyoneWon(arr))
      {
        case 0: if(moveCounter==9)
                {
                  gtk_button_set_label(statusClick, "Empate");
                  gameNotOver=0; return 0;
                } break;
        case 1: gtk_button_set_label(statusClick, "Ganó Jugador 1");
                gameNotOver=0; return 0;
        case 2: gtk_button_set_label(statusClick, "Ganó Jugador 2");
                gameNotOver=0; return 0;
      }
      gtk_button_set_label(statusClick, "Turno del jugador 2");
    }
    // Se ejecuta cuando se presiona el boton durante el movimiento uniforme
    else
    {
      arr[0][2]=2;
      gtk_button_set_label(buttonTemp, "O");
      switch(hasAnyoneWon(arr))
      {
        case 1: gtk_button_set_label(statusClick, "PLAYER 1 WON");
                gameNotOver=0; return 0;
        case 2: gtk_button_set_label(statusClick, "PLAYER 2 WON");
                gameNotOver=0; return 0;
      }
      gtk_button_set_label(statusClick, "Turno del jugador 1");
    }
    pressed[0][2]=1;
  }
  return 0;
}

// Boton en la posicion 21
int on_button21_clicked(GtkWidget *click_button, gpointer   user_data)
{
  GtkButton *buttonTemp = (GtkButton *) user_data;

  // Si esta en modo inicializacion se ejecuta
  if(initialise!=0)
  {
    initialising(buttonTemp,1,0);
    return 0;
  }

  // Se ejecuta si el boton se presiona mientras se esta inicializando el juego
  if(pressed[1][0]) return 0;

  // Se ejecuta cuando se presiona el boton en el juego
  if(flag&&gameNotOver)
  {
    // Se ejecuta durante el movimiento impar
    if(++moveCounter%2)
    {
      arr[1][0]=1;
      gtk_button_set_label(buttonTemp, "X");
      pressed[1][0]=1;
      switch(hasAnyoneWon(arr))
      {
        case 0: if(moveCounter==9)
                {
                  gtk_button_set_label(statusClick, "Empate");
                  gameNotOver=0; return 0;
                } break;
        case 1: gtk_button_set_label(statusClick, "Ganó Jugador 1");
                gameNotOver=0; return 0;
        case 2: gtk_button_set_label(statusClick, "Ganó Jugador 2");
                gameNotOver=0; return 0;
      }
      gtk_button_set_label(statusClick, "Turno del Jugador 2");
    }
    // Se ejecuta cuando se presiona el boton durante el movimiento uniforme
    else
    {
      arr[1][0]=2;
      gtk_button_set_label(buttonTemp, "O");
      switch(hasAnyoneWon(arr))
      {
        case 1: gtk_button_set_label(statusClick, "PLAYER 1 WON");
                gameNotOver=0; return 0;
        case 2: gtk_button_set_label(statusClick, "PLAYER 2 WON");
                gameNotOver=0; return 0;
      }
      gtk_button_set_label(statusClick, "Turno del jugador 1");
    }
    pressed[1][0]=1;
  }
  return 0;
}

// Boton en la posicion 22
int on_button22_clicked(GtkWidget *click_button, gpointer   user_data)
{
  GtkButton *buttonTemp = (GtkButton *) user_data;

  // Si esta en modo inicializacion se ejecuta
  if(initialise!=0)
  {
    initialising(buttonTemp,1,1);
    return 0;
  }

  // Se ejecuta si el boton se presiona mientras se esta inicializando el juego
  if(pressed[1][1]) return 0;

  // Se ejecuta cuando se presiona el boton en el juego
  if(flag&&gameNotOver)
  {
    // Se ejecuta durante el movimiento impar
    if(++moveCounter%2)
    {
      arr[1][1]=1;
      gtk_button_set_label(buttonTemp, "X");
      pressed[1][1]=1;
      switch(hasAnyoneWon(arr))
      {
        case 0: if(moveCounter==9)
                {
                  gtk_button_set_label(statusClick, "Empate");
                  gameNotOver=0; return 0;
                } break;
        case 1: gtk_button_set_label(statusClick, "Ganó Jugador 1");
                gameNotOver=0; return 0;
        case 2: gtk_button_set_label(statusClick, "Ganó Jugador 2");
                gameNotOver=0; return 0;
      }
      gtk_button_set_label(statusClick, "Turno del jugador 2");
    }
    // Se ejecuta cuando se presiona el boton durante el movimiento uniforme
    else
    {
      arr[1][1]=2;
      gtk_button_set_label(buttonTemp, "O");
      switch(hasAnyoneWon(arr))
      {
        case 1: gtk_button_set_label(statusClick, "Ganó Jugador 1");
                gameNotOver=0; return 0;
        case 2: gtk_button_set_label(statusClick, "Ganó Jugador 2");
                gameNotOver=0; return 0;
      }
      gtk_button_set_label(statusClick, "Turno del Jugador 1");
    }
    pressed[1][1]=1;
  }
  return 0;
}

// Boton en la posicion 23
int on_button23_clicked(GtkWidget *click_button, gpointer   user_data)
{
  GtkButton *buttonTemp = (GtkButton *) user_data;

  // Si esta en modo inicializacion se ejecuta
  if(initialise!=0)
  {
    initialising(buttonTemp,1,2);
    return 0;
  }

  // Se ejecuta si el boton se presiona mientras se esta inicializando el juego
  if(pressed[1][2]) return 0;

  // Se ejecuta cuando se presiona el boton en el juego
  if(flag&&gameNotOver)
  {
    // Se ejecuta durante el movimiento impar
    if(++moveCounter%2)
    {
      arr[1][2]=1;
      gtk_button_set_label(buttonTemp, "X");
      pressed[1][2]=1;
      switch(hasAnyoneWon(arr))
      {
        case 0: if(moveCounter==9)
                {
                  gtk_button_set_label(statusClick, "Empate");
                  gameNotOver=0; return 0;
                } break;
        case 1: gtk_button_set_label(statusClick, "Ganó Jugador 1");
                gameNotOver=0; return 0;
        case 2: gtk_button_set_label(statusClick, "Ganó Jugador 2");
                gameNotOver=0; return 0;
      }
      gtk_button_set_label(statusClick, "Turno del Jugador 2");
    }
    // Se ejecuta cuando se presiona el boton durante el movimiento uniforme
    else
    {
      arr[1][2]=2;
      gtk_button_set_label(buttonTemp, "O");
      switch(hasAnyoneWon(arr))
      {
        case 1: gtk_button_set_label(statusClick, "Ganó Jugador 1");
                gameNotOver=0; return 0;
        case 2: gtk_button_set_label(statusClick, "Ganó Jugador 2");
                gameNotOver=0; return 0;
      }
      gtk_button_set_label(statusClick, "Turno del jugador 1");
    }
    pressed[1][2]=1;
  }
  return 0;
}

// Boton en la posicion 31
int on_button31_clicked(GtkWidget *click_button, gpointer   user_data)
{
  GtkButton *buttonTemp = (GtkButton *) user_data;

  // Si esta en modo inicializacion se ejecuta
  if(initialise!=0)
  {
    initialising(buttonTemp,2,0);
    return 0;
  }

  // Se ejecuta si el boton se presiona mientras se esta inicializando el juego
  if(pressed[2][0]) return 0;

  // Se ejecuta cuando se presiona el boton en el juego
  if(flag&&gameNotOver)
  {
    // Se ejecuta durante el movimiento impar
    if(++moveCounter%2)
    {
      arr[2][0]=1;
      gtk_button_set_label(buttonTemp, "X");
      pressed[2][0]=1;
      switch(hasAnyoneWon(arr))
      {
        case 0: if(moveCounter==9)
                {
                  gtk_button_set_label(statusClick, "Empate");
                  gameNotOver=0; return 0;
                } break;
        case 1: gtk_button_set_label(statusClick, "Ganó Jugador 1");
                gameNotOver=0; return 0;
        case 2: gtk_button_set_label(statusClick, "Ganó Jugador 2");
                gameNotOver=0; return 0;
      }
      gtk_button_set_label(statusClick, "Turno del Jugador 2");
    }
    // Se ejecuta cuando se presiona el boton durante el movimiento uniforme
    else
    {
      arr[2][0]=2;
      gtk_button_set_label(buttonTemp, "O");
      switch(hasAnyoneWon(arr))
      {
        case 1: gtk_button_set_label(statusClick, "Ganó Jugador 1");
                gameNotOver=0; return 0;
        case 2: gtk_button_set_label(statusClick, "Ganó Jugador 2");
                gameNotOver=0; return 0;
      }
      gtk_button_set_label(statusClick, "Turno del Jugador 1");
    }
    pressed[2][0]=1;
  }
  return 0;
}

// Boton en la posicion 32
int on_button32_clicked(GtkWidget *click_button, gpointer   user_data)
{
  GtkButton *buttonTemp = (GtkButton *) user_data;

  // Si esta en modo inicializacion se ejecuta
  if(initialise!=0)
  {
      initialising(buttonTemp,2,1);
      return 0;
  }

  // Se ejecuta si el boton se presiona mientras se esta inicializando el juego
  if(pressed[2][1]) return 0;

  // Se ejecuta cuando se presiona el boton en el juego
  if(flag&&gameNotOver)
  {
    // Se ejecuta durante el movimiento impar
    if(++moveCounter%2)
    {
      arr[2][1]=1;
      gtk_button_set_label(buttonTemp, "X");
      pressed[2][1]=1;
      switch(hasAnyoneWon(arr))
      {
        case 0: if(moveCounter==9)
                {
                  gtk_button_set_label(statusClick, "Empate");
                  gameNotOver=0; return 0;
                } break;
        case 1: gtk_button_set_label(statusClick, "Ganó Jugador 1");
                gameNotOver=0; return 0;
        case 2: gtk_button_set_label(statusClick, "Ganó Jugador 2");
                gameNotOver=0; return 0;
      }
      gtk_button_set_label(statusClick, "Turno del Jugador 2");
    }
    // Se ejecuta cuando se presiona el boton durante el movimiento uniforme
    else
    {
      arr[2][1]=2;
      gtk_button_set_label(buttonTemp, "O");
      switch(hasAnyoneWon(arr))
      {
        case 1: gtk_button_set_label(statusClick, "Ganó Jugador 1");
                gameNotOver=0; return 0;
        case 2: gtk_button_set_label(statusClick, "Ganó Jugador 2");
                gameNotOver=0; return 0;
      }
      gtk_button_set_label(statusClick, "Turno del Jugador 1");
    }
    pressed[2][1]=1;
  }
  return 0;
}

// Boton en la posicion 33
int on_button33_clicked(GtkWidget *click_button, gpointer   user_data)
{
  GtkButton *buttonTemp = (GtkButton *) user_data;

  // Si esta en modo inicializacion se ejecuta
  if(initialise!=0)
  {
      initialising(buttonTemp,2,2);
      return 0;
  }

  // Se ejecuta si el boton se presiona mientras se esta inicializando el juego
  if(pressed[2][2]) return 0;

  // Se ejecuta cuando se presiona el boton en el juego
  if(flag&&gameNotOver)
  {
    // Se ejecuta durante el movimiento impar
    if(++moveCounter%2)
    {
      arr[2][2]=1;
      gtk_button_set_label(buttonTemp, "X");
      pressed[2][2]=1;
      switch(hasAnyoneWon(arr))
      {
        case 0: if(moveCounter==9)
                {
                  gtk_button_set_label(statusClick, "Empate");
                  gameNotOver=0; return 0;
                } break;
        case 1: gtk_button_set_label(statusClick, "Ganó Jugador 1");
                gameNotOver=0; return 0;
        case 2: gtk_button_set_label(statusClick, "Ganó Jugador 2");
                gameNotOver=0; return 0;
      }
      gtk_button_set_label(statusClick, "Turno del jugador 2");
    }
    // Se ejecuta cuando se presiona el boton durante el movimiento uniforme
    else
    {
      arr[2][2]=2;
      gtk_button_set_label(buttonTemp, "O");
      switch(hasAnyoneWon(arr))
      {
        case 1: gtk_button_set_label(statusClick, "Ganó Jugador 1");
                gameNotOver=0; return 0;
        case 2: gtk_button_set_label(statusClick, "Ganó Jugador 2");
                gameNotOver=0; return 0;
      }
      gtk_button_set_label(statusClick, "Turno del Jugador 1");
    }
    pressed[2][2]=1;
  }
  return 0;
}

// Funcion para verificar si las filas y columnas estan llenas con 0 o 1
int hasAnyoneWon(int a[3][3])
{
  int oneWon=0; int twoWon=0; int i,j;
  for(i=0;i<3;i++)
  {
    for(j=0;j<3;j++)
    {
      if(a[i][j]==1) oneWon++;
      if(a[i][j]==2) twoWon++;
    }
    if(oneWon==3) return 1;
    if(twoWon==3) return 2;
    oneWon=0; twoWon=0;
  }
  for(i=0;i<3;i++)
  {
    for(j=0;j<3;j++)
    {
      if(a[j][i]==1) oneWon++;
      if(a[j][i]==2) twoWon++;
    }
    if(oneWon==3) return 1;
    if(twoWon==3) return 2;
    oneWon=0; twoWon=0;
  }
  for(i=0;i<3;i++)
  {
    if(a[i][i]==1) oneWon++;
    if(a[i][i]==2) twoWon++;
  }
  if(oneWon==3) return 1;
  if(twoWon==3) return 2;
  oneWon=0; twoWon=0;
  for(i=0,j=2;i<3;i++,j--)
  {
    if(a[i][j]==1) oneWon++;
    if(a[i][j]==2) twoWon++;
  }
  if(oneWon==3) return 1;
  if(twoWon==3) return 2;
  return 0;
}

/* Reestablece los botones, despues de que se estable completamente el modo de
inicializacion */
void setAllButtonsToBlank()
{
    int i,j;
    for(i=0;i<3;i++)
    {
        for(j=0;j<3;j++)
        {
            gtk_button_set_label(button[i][j], " ");
        }
    }
}

// Funcion se ejecuta cuando se presiona por primera vez cada boton
void initialising(GtkButton *buttonInit,int i,int j)
{
    if(button[i][j]!=NULL) return;
    button[i][j]=buttonInit;
    gtk_button_set_label(button[i][j], "OK");
    if(++initialise==10)
    {
      initialise=0;
      gtk_button_set_label(statusClick, "Turno del Jugador 1");
      setAllButtonsToBlank();
    }
}
