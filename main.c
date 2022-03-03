#include <gtk/gtk.h>
#include <stdlib.h>

// function prototyping
void initialising(GtkButton *buttonInit,int i,int j);
int hasAnyoneWon(int a[3][3]);
void setAllButtonsToBlank();

// declaring global variables
static GtkButton *statusClick,*gameClick;
static GtkButton *button[3][3]={{NULL,NULL,NULL},{NULL,NULL,NULL},{NULL,NULL,NULL}};

// declaring global flags
static int flag=0, gameNotOver=1, initialise=0, statusFlag=0, pressed[3][3]={{0,0,0},{0,0,0},{0,0,0}};
static int moveCounter=0;

// declaring game parameters
static int arr[3][3]={{0,0,0},{0,0,0},{0,0,0}};
static int gameType=0;

// main method, start of execution
int main(int argc, char *argv[])
{
    GtkBuilder      *builder;
    GtkWidget       *window;

    gtk_init(&argc, &argv);

// initialising GTKbuilder with .glade file
    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "mainUI.glade", NULL);

// initialising main window widget
    window = GTK_WIDGET (gtk_builder_get_object(builder, "Juego"));
    gtk_builder_connect_signals(builder, NULL);

// dereferencing builder pointer (for reuse later)
    g_object_unref(builder);

// showing main window widget
    gtk_widget_show(window);
    gtk_main();
    return 0;
}

// called when quit is selected in settings menu
void on_gameExit_activate()
{
    gtk_main_quit();
    exit(0);
}

// runs the about dialog box
void on_aboutSelection_activate()
{
  GtkWidget   *about;
  GtkBuilder  *builder;

// initialising GTKbuilder with .glade file
  builder = gtk_builder_new();
  gtk_builder_add_from_file (builder, "mainUI.glade", NULL);

// initialising about widget
  about = GTK_WIDGET(gtk_builder_get_object(builder, "instruccion"));
  gtk_builder_connect_signals(builder, NULL);

// dereferencing builder for later use
  g_object_unref(builder);
  gtk_dialog_run (GTK_DIALOG (about));

// when response signal is triggered
  gtk_widget_destroy (about);
}

// when gamemode button is pressed
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
  flag=1; //enable flag at button press, not here. reset flag to 0 in restart
  gtk_button_set_label(statusClick, "Jugar");
  GtkBuilder  *builder;
  GtkWidget   *gameDialog;
// initialising GTKbuilder with .glade file
  builder = gtk_builder_new();
  gtk_builder_add_from_file (builder, "mainUI.glade", NULL);
// initialising gamemode selection widget
  gameDialog = GTK_WIDGET(gtk_builder_get_object(builder, "vent_lis"));
  gtk_builder_connect_signals(builder, NULL);
  g_object_unref(builder);
  gtk_dialog_run (GTK_DIALOG (gameDialog));
// when window is closed from the x button in toolbar
  gtk_widget_destroy(gameDialog);
}

// runs when PvP is selected in settings menu
void on_listo_clicked(GtkWidget *click_button, gpointer   user_data)
{
  gameType = 0;
  gtk_widget_destroy((GtkWidget *) user_data);
  gtk_button_set_label(gameClick, "Vamos");
}


/* runs when restart is selected in settings menu. Destroys old window,
 reinitialises all global variables and runs main method */
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


/* initialising mode is used to get a reference to all the buttons in order to change
their labels in PvC mode. This is a downside of using Glade, as the computer
can not press the button when it's its move */

// runs when button 1,1 is clicked
int on_button11_clicked(GtkWidget *click_button, gpointer   user_data)
{
  GtkButton *buttonTemp = (GtkButton *) user_data;

  // runs if in initialising mode
  if(initialise!=0)
  {
    initialising(buttonTemp,0,0);
    return 0;
  }

  // runs if button has already been pressed either while Initialising or in-game
  if(pressed[0][0]) return 0;

  // runs when button is pressed in-game
  if(flag&&gameNotOver)
  {
    // runs when the button is pressed during an odd move
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
    // runs when the button is pressed during an even move
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

// runs when button 1,2 is clicked
int on_button12_clicked(GtkWidget *click_button, gpointer   user_data)
{
  GtkButton *buttonTemp = (GtkButton *) user_data;

  // runs if in initialising mode
  if(initialise!=0)
  {
    initialising(buttonTemp,0,1);
    return 0;
  }

  // runs if button has already been pressed either while Initialising or in-game
  if(pressed[0][1]) return 0;

  // runs when button is pressed in-game
  if(flag&&gameNotOver)
  {
    // runs when the button is pressed during an odd move
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
    // runs when the button is pressed during an even move
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

// runs when button 1,3 is clicked
int on_button13_clicked(GtkWidget *click_button, gpointer   user_data)
{
  GtkButton *buttonTemp = (GtkButton *) user_data;

  // runs if in initialising mode
  if(initialise!=0)
  {
    initialising(buttonTemp,0,2);
    return 0;
  }

  // runs if button has already been pressed either while Initialising or in-game
  if(pressed[0][2]) return 0;

  // runs when button is pressed in-game
  if(flag&&gameNotOver)
  {
    // runs when the button is pressed during an odd move
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
    // runs when the button is pressed during an even move
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

// runs when button 2,1 is clicked
int on_button21_clicked(GtkWidget *click_button, gpointer   user_data)
{
  GtkButton *buttonTemp = (GtkButton *) user_data;

  // runs if in initialising mode
  if(initialise!=0)
  {
    initialising(buttonTemp,1,0);
    return 0;
  }

  // runs if button has already been pressed either while Initialising or in-game
  if(pressed[1][0]) return 0;

  // runs when button is pressed in-game
  if(flag&&gameNotOver)
  {
    // runs when the button is pressed during an odd move
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
    // runs when the button is pressed during an even move
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

// runs when button 2,2 is clicked
int on_button22_clicked(GtkWidget *click_button, gpointer   user_data)
{
  GtkButton *buttonTemp = (GtkButton *) user_data;

  // runs if in initialising mode
  if(initialise!=0)
  {
    initialising(buttonTemp,1,1);
    return 0;
  }

  // runs if button has already been pressed either while Initialising or in-game
  if(pressed[1][1]) return 0;

  // runs when button is pressed in-game
  if(flag&&gameNotOver)
  {
    // runs when the button is pressed during an odd move
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
    // runs when the button is pressed during an even move
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

// runs when button 2,3 is clicked
int on_button23_clicked(GtkWidget *click_button, gpointer   user_data)
{
  GtkButton *buttonTemp = (GtkButton *) user_data;

  // runs if in initialising mode
  if(initialise!=0)
  {
    initialising(buttonTemp,1,2);
    return 0;
  }

  // runs if button has already been pressed either while Initialising or in-game
  if(pressed[1][2]) return 0;

  // runs when button is pressed in-game
  if(flag&&gameNotOver)
  {
    // runs when the button is pressed during an odd move
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
    // runs when the button is pressed during an even move
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

// runs when button 3,1 is clicked
int on_button31_clicked(GtkWidget *click_button, gpointer   user_data)
{
  GtkButton *buttonTemp = (GtkButton *) user_data;

  // runs if in initialising mode
  if(initialise!=0)
  {
    initialising(buttonTemp,2,0);
    return 0;
  }

  // runs if button has already been pressed either while Initialising or in-game
  if(pressed[2][0]) return 0;

  // runs when button is pressed in-game
  if(flag&&gameNotOver)
  {
    // runs when the button is pressed during an odd move
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
    // runs when the button is pressed during an even move
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

// runs when button 3,2 is clicked
int on_button32_clicked(GtkWidget *click_button, gpointer   user_data)
{
  GtkButton *buttonTemp = (GtkButton *) user_data;

  // runs if in initialising mode
  if(initialise!=0)
  {
      initialising(buttonTemp,2,1);
      return 0;
  }

  // runs if button has already been pressed either while Initialising or in-game
  if(pressed[2][1]) return 0;

  // runs when button is pressed in-game
  if(flag&&gameNotOver)
  {
    // runs when the button is pressed during an odd move
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
    // runs when the button is pressed during an even move
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

// runs when button 3,3 is clicked
int on_button33_clicked(GtkWidget *click_button, gpointer   user_data)
{
  GtkButton *buttonTemp = (GtkButton *) user_data;

  // runs if in initialising mode
  if(initialise!=0)
  {
      initialising(buttonTemp,2,2);
      return 0;
  }

  // runs if button has already been pressed either while Initialising or in-game
  if(pressed[2][2]) return 0;

  // runs when button is pressed in-game
  if(flag&&gameNotOver)
  {
    // runs when the button is pressed during an odd move
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
    // runs when the button is pressed during an even move
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

/* function for checking if whether any row, column or diagonal is completely
filled by either 0, 1, or 2 and returns either 0,1 or 2 */
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

// called after initialising mode is completed to reset buttons
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

// runs for the first time each button is pressed
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
