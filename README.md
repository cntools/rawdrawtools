# rawdrawtools

Tool to create custom fonts for rawdraw's CNFGDrawNiceText function.

Controls: 
* WASD to select the character to edit
* Click a dot in the main grid to start a line/add a line to the segment.
* Click again the same point to end the segment (to make a line that's not connected or a point).
* Right click to remove the last point.
	
Position Offset selection grid: each square represents a pixel of difference (that scales with the font), with the blue square being the default position and the green square the current one.

Width selector: Represents how wide the character will be when drawn.

When closing it'll override the FontData.c file.
When selecting another character it'll save into FontDataBackup.c.

To load the changes the next time you open the program, you will need to recompile text.c, otherwise you will load the old data and potentially lose your last session's changes.
