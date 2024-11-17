Designed by A/Prof Soh WS
CG1111A Engineering Principles & Practice I
The A-maze-ing Race Project 2024

Introduction
Welcome to the grand project of this course: The A-maze-ing Race!
In this race, your mBot needs to find its way through a maze in the shortest time. Similar to its
namesake TV program, your mBot will be facing a number of challenges at intermediate waypoints
while attempting to complete the race. To successfully meet all the requirements, you need to have
a good grasp of many of the principles you have learnt in this course and apply them into good
practice!

Key Project Requirements
1. The mBot must not bump into any wall. Your mBot shall accomplish this with the help of one
ultrasonic sensor on one side, and one infrared (IR) proximity sensor on the other side (no
restriction on which of these two sensors to place on the left or right). You need to come up
with your own algorithms to meet this requirement. Note that there will be penalty points if
your mBot does not utilize the IR sensor. There will also be penalty points for bumping into
walls (including wires brushing against the wall).

2. When your mBot is not making a turn, it must travel as straight as possible. (It must not drive
in a zig-zag manner like a car driven by a drunken driver.)

3. All turns in the maze are dictated by “waypoint challenges”. Your mBot must not make any
automatic turn without decoding a waypoint challenge.

4. When making a turn, your mBot must not over- or under-manoeuvre too much.
   
5. At each waypoint challenge, there will be a black strip on the maze floor. Your mBot needs to
detect the black strip, stop, solve the waypoint challenge directly underneath it, and act
according to the turn instruction decoded from the waypoint challenge.

6. Waypoint Challenge: Colour-sensing
You need to build your own colour-sensing circuit on a mini-breadboard, and place it
underneath your mBot. At each waypoint challenge grid, besides the black strip, there will be
a colour paper directly underneath your mBot. (Please refer to the video “Black Strip and
Colour Paper's Positions.mp4” in Canvas for an illustration of how the black strip and the
colour paper will be placed.) Depending on the colour of the paper, your mBot needs to
execute one of the following five types of turns:

![Screenshot 2024-11-17 084246](https://github.com/user-attachments/assets/e2f27035-95de-40a3-9352-89362f776dae)

Figure 1: The five types of turns that your mBot needs to execute.

Colour Interpretation
Red Left-turn
Green Right turn
Orange 180° turn within the same grid
Pink Two successive left-turns in two grids
Light Blue Two successive right-turns in two grids

Note:
For the “two successive left-turns in two grids” and the “two successive right-turns in two
grids”, there will not be any black strip in the second grid to guide the mBot to execute the
second turn. Your mBot needs to be hard coded to make these successive turns.

7. End of Maze:
At the end of the maze, there will also be a black strip. The colour of the paper underneath
the mBot at this grid will be white. Upon decoding that it has reached the end of the conquest,
the mBot must stop moving, and play a celebratory tune of your choice (Yay!).

Key Knowledge and Skills Needed
• DC Circuit Principles
• Arduino/mBot programming (self-learn)
• Analog-to-digital conversion
• How to read/interpret datasheets
• Circuit building skills
• Principles of IR proximity sensor
• Principles of colour sensor
• Principles of ultrasonic range sensor
• Hardware wiring and debugging skills
• Equipment usage (multimeter, etc.)

Project Evaluation
1. Mock Evaluation: Week 13 Studio A timeslot
This will be a good gauge of your mBot’s readiness. The evaluation procedure will be the same
as the final evaluation. Hence, you should get your mBot ready as if it is the final evaluation,
so that you know what are your mBot’s shortfalls and improve upon them.

2. Final Evaluation: Week 13 Studio B timeslot

Rules
1. At each challenge, if your mBot turns in the wrong direction, you will teleport it to one grid
before the challenge to make a second attempt, and if necessary, a third attempt, while the
clock continues to run. If it fails at the third attempt, you shall manually turn it to the correct
direction and let it continue with the rest of the maze.

2. If your mBot gets stuck to a wall, you can move it to the correct position within the same grid.
  
3. You are not allowed to add any commercial-off-the-shelf sensors that are not issued by us.

4. The teaching team will set up one maze table on the final evaluation day. This maze layout
will not be revealed beforehand. The figure below shows a sample maze layout.

![Screenshot 2024-11-17 084408](https://github.com/user-attachments/assets/3966caec-3e1b-46e2-8ac6-a8df8a36e9ec)

5. As can be seen in the sample maze layout, some of the walls may be missing. Your mBot
must be able to handle such missing walls(e.g., continue going straight even if one or two side
walls is/are missing).

Note:
• There will always be a maze wall in front of the mBot within a waypoint challenge grid.

6. Your mBot will be tested in two maze tables – one set up by us on the evaluation day, and
another existing maze within the lab – to ensure that you did not overly tune your mBot to
work in just one maze. Your project demo marks will be the average of your mBot’s
performance in these two mazes.

7. You are not allowed to perform any calibration at the start of your project demo. Hence, all
calibrations and software upload must have been completed before surrendering the mBots
to the instructors.

8. You are allowed to fine-tune your mBot's hardware and software and go for a re-run of the
demo if the following conditions occur, subject to a 20% penalty of the total demo marks:
a. If your mBot fails to decode more than half of the colour challenges.
b. If your mBot keeps bumping into walls or getting stuck in the maze.

Grading Criteria
Criterion Marks
Project Demo 25
Neatness of wirings and robot 5
Algorithms and coding (e.g., elegance of
algorithms, well commented codes, etc.)
10
Short team report 10
Total 50

The following are the key grading criteria during the project demo:
• Number of bumps into the maze walls (regardless of whether it gets stuck); note
that wires brushing against the wall are also regarded as bumps.
• Successfully decoding the challenges: for each challenge, your mBot has up to 3
tries. You get full marks for a challenge if you succeed in the 1st attempt; some
penalty marks will be incurred if you succeed in the 2nd or the 3rd attempt.
• Whether your mBot utilizes the IR proximity sensor (we will test it).
• Whether your mBot can travel in a near-straight line when not turning.
• Whether your mBot can execute a turn accurately without over- or undermanoeuvring too much.
• Whether the mBot plays a celebratory tone upon detecting the end of the maze, and
stops moving.
• Total time taken to complete the maze.
