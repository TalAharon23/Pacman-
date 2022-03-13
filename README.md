#     PACMAN GAME   
     by Tal Aharon

This game was developed as part of a bachelor's degree in computer science at Tel Aviv-Jaffa College.
The game developed with C++, using object-oriented programming, polymorphism and files.

### The Pacman has 3 game options:
1) Simple game mode:
   - Reading one or more gameboard templates from the project folder and playing without saving.
3) Saving game mode: 
   - Reading one or more gameboard templates from the project folder. 
   - During the game and according to the playing board, the user's steps and game's results are saving into a file. 
4) Load mode: 
   - View the whole saved game from a file and display it to the screen.
6) Silent mode - testing the game:
   - Reading the game's steps and results.
   - Checking in front of the files that everything is done properly and that the game board is correct.
   - Printing the test's result (succeed or not).

 ### Running steps:
 Open CMD and navigate to pacman's folder and run:
  * `pacman.exe -simple` for simple game mode.
  * `pacman.exe -save` for saving game mode.
  * `pacman.exe -load` for load mode.
  * `pacman.exe -load -silent` for silent mode.

### Simple and Load modes example:
![Pacman Gif](https://user-images.githubusercontent.com/82831070/158054613-69f7166f-e0ab-4512-b422-cd28723e1a57.gif)
