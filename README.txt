1.Name - GAURAV SAXENA
2.List of Files:
	a.  makefile	- Makefile for the compilation of the program and cleanup.	
	b.  shared.h	- The header file for shared memory and structures declaration and  to include libraries to include.
	c.  manager.c	- The manager of the team for hosting the match, and updating the points for scoreboard.
	d.  main.c  - The main function for creating shared schedule, creating managers for team, and maintaing scoreboard.
    e.  sample  - The sample input file containg no of teams and schedule of matches.
3.How to Compile
    command: make
4.HOW to RUN
    command: ./master.out <input file name>
5.Description of the program
    The program reads no. of teams and schedule from the input file, and creates a struct shared memory copy of it.
    It then creates struct scoreboard shared memory, and managers for the Teams.
    The managers wil go thorugh the match schedule and try to start the match for their home team.
    This will happen only after the manager has checked the status of home team and away team from the critical section shared memory array called busy.
    This array - busy, keeps the current status of teams - currently playing or free.
    After a match is completed, manager updates the points in scoreboard.
    Once all the matches are completed all the managers terminate, and the control goes back to the main program, which was
    waiting for all the managers to end. The main program then sorts the teams in highest ranking order in the scoreboard, and 
    displays it.
