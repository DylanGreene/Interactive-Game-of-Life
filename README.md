# Interactive-Game-of-Life

I built this version of the game of life using gfx5. However, I also added two
functions to the file, which I have renamed gfx_custom.c. The two functions are
used to return the current x and y positions relative to the gfx window at any
time, even when an event has not occurred. The purpose of these functions was to
be able to constantly track the position of the mouse continuously so that I
could draw a temporary cell snapped to the grid at the mouses location. This
would allow the user to visualize where they were going to place or remove a
cell. The functions, current_xpos() and current_ypos() are placed near the top
of the gfx_custom.c file. Both functions take advantage of the XQueryPointer()
function in Xlib. I pass this function the global variables gfx_display and
gfx_window. I also pass by reference several other variables which I have
declared. All of these other variables are a part of the return of the function.
For my purposes, I only care about the childx and childy. These are the mouse
pointer coordinates relative to the gfx_window.

Moving on to the user's perspective of the program:
The user has the option of typing the executable alone, or the executable and a
scene file. The scene file contains lines of commands to place live cells
followed by a continuously play command. If the user does not include a scene
file, the user can press 'a' to add new live cells. Each time they press 'a' a
green cell will follow their mouse showing them where the cell will be placed
when they click. Once they click, the cell will be placed at that position. If
the user places a cell in an incorrect position, they can click 'r'. When 'r'
is pressed, a red square will follow their mouse until they click on a live
cell. Once the user clicks on a live cell, the cell will be killed and removed.
Once the user has placed all of the cells they want, they can press 'n' to
iterate tick by tick. Alternatively, they can press 'p' to play a continuous
animation of updating the cells. Additionally, while the continuous animation is
playing the user can press 's' at any time to stop the continual playing. Once
's' is pressed, the user can again press any of the aforementioned keys in
order to add, remove, iterate, or play continuously again. Also, 'q' can be
pressed to quit the program so long as the user is not in one of the
subfunctions.

From my perspective, I built this program to have some features which were not
actually acquired. Initially, I aquire the number of command line arguments used
when the user types and enters the executable. If there are exactly two
arguments (the executable and a scene file), I attempt to open the file to the
file pointer fp. If the file is not open successfully, I inform the user and
stop the program. If the file is opened successfully, the setSceneFileState
function is called which basically reads in the commands in the file and updates
the liveCell array with the specified cells. Once the 'a' commands have
concluded in the file, the 'p' commmand is executed at which point the
playContinuously function is called. This function acts to call the other main
logic function: iterateSimulation. This function calls the updateCellStates
function which applies the actual game of life rules. Once the states have been
updated, the cells are drawn in white by calling the drawLiveCell function which
simply draws a filled square on the grid using the gfx_fill_rectangle function.
Other than that, the rest of the program follows the description I gave above
about my gfx functions.

The output of this program was verified by checking the drawings against known 
game of life patterns. Additionally, all of the key presses and file input was
tested extensively.
