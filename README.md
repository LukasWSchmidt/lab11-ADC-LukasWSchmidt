# Lab10 - Lot's of Lights (SPI)

## 1. Introduction

The purpose of this lab is to explore the design of firmware to control an LED light array via an SPI Connection.  To test the firmware a demo application will be created.

## 2. Prerequisites

- A completed development board.
- A wired LED Light array

## 3. Activities


### 3.1 Create Project

1. Clone this project into your CubeIDE workspace directory with GitHub Desktop, WSL Git, or Windows Git.
2. In CubeIDE copy the Lab8 Project in the ProjectExplorer tab by copy-and-pasting using the same project name as the cloned directory.
3. Modify the rubric file adding your name and section number.
4. Make an initial commit.
5. Start working.
6. As you modify previous driver files be sure to updated the filed comment blocks.

### 3.2 Write SPI API 

- Write an spi.h and spi.c api to interact with the spi component.
- Test your api using an analog discovery
	
### 3.3 Utilize the SPI API to build firmware to control the LED Array.

- Create led_array.h and led_array.c api.
- Utilize a file static frame buffer keep a local copy of the frame
- Include the following functions in your api
	- LedArrayInit
	- LedArraySetPixel(x,y,val)
	- LedArrayClear
	- LedArrayUpdate
- Note the display should only updated when LedArrayUpdate is called.
	
### 3.4 Create an application in main.c

- Create an application that exercises your api
	- Game 
	- Animation
	- Anything you can think of

### 3.5 Extra Credit (for +5)

- Make your LedArrayUpdate Interrupt driven

## 4. Deliverables

- Demonstrate your working project to your instructor or lab assistant at beginning of Lab Week11.
- Commit **only** your "inc" and "src" directories
	- You may add any extra files to you *.gitignore* file to prevent future inclusion
- Use git to back up your work and track commits.
- Your final submission should be on the main branch
	- Clearly label the commit as "Lab10 Submission"
- Be sure to update all documentation.
