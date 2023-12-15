# 3081 Homework 4
   _Team 001-03_
   Nathan Kjaer (kjaer010) and Ben Pipkorn (pipko016)

## What is the project about

This project is a simulation of a drone delivery system. It combines HTML, Javascript, and C++ to make a cohesive program that allows a user to schdeule deliveries, and follow different objects that spawn into the simulation. Multiple objects are able to spawn within the simulation, 3 of them being user generated. The ones that are unable to be user generated are drones and helicopters. There is a single helicopter flying around the simualtion and is there simply for decorative purposes. The drone that spawns into the simulation is the central point of the project. The drone makes all deliveries that the user schedules as well as does a little celebration when it has completed a delivery. The objects that are user generated include humans, packages, and robots. The humans are much like helicopters in that they provide no extra function to the program other than that they decorate the simulation. Packages and robots are spawned simultaneously and are created when a user schedules a delivery.

## How to use the simulation

### How to compile and connect to the simulation
The simulation can be compiled and run using 2 commands in the termainal. 
Please make sure you are in the top-most project directory when running these commands.
The first command you should run will be a simple
   ```bash
   make -j
   ```
command. This will compile the project and will put all generated files in the /build folder. The next command that you will have to run will start up the simulation itself and will not stop running until the Stop Simulation button is pressed. This command will be
   ```bash
   ./build/bin/transit_service 8081 apps/transit_service/web/
   ```
The first part runs the simulation, the second part (8081) is the port that you can use to connect to the simulation on your local machine. The last part is the web application that is run with the simulation. This can be done by connecting to
https://127.0.0.1:8081
on your local web browser, if you are using ssh by sure to foreward the port to your local machine.

### User controls of the simulation
Now that we are connected to the simulation and it is up and running, and we are connected using the link above, there will be 3 prespawned objects (a drone, a human, and a helicopter), and a UI box in the top right corner. This UI box is where you can see how many entities are in the simulation, as well as select which one of those entities you want to follow. There is also a simulation speed slider that will control the speed at which the simulation runs. There are also a multitude of buttons below the slider that have different functions. 

One of the buttons leads to a scheduling page, this page is where the user is able to spawn extra humans into the simulation. This page also allows the user to schedule trips by naming the trip, selecting the path algorithm that the drone will use, as well as stop and start points that are to be selected on the map in the center of the screen. This page also displays current and past trips on the right side of the screen.
There are 3 other buttons in the UI box, one is used to stop the simulation, another is used to save the current state of the simulation into a file in the /saves folder within th eproject directory. The last button allows you to restore a simulation from the /saves folder into the current simulation.

## What the simulation does 

### Entity objects
The simulation updates all entity objects individually, this include things such as movement, bound checking, and checking or availability. Drones update by checking if its current delivery is finished and if it is then it either takes another delivery if one is availible otherwise it waits for one to come in. Packages check if they are attached to a drone and if it is, it updates with the drone. Humans and Helicopters only update their position. Robots do not update.

### Simulation model
The simulation model takes care of updating all of the entities by calling all update functions, as well as removes objects from the simulation that need to be removed. 

### Transit Service
The transit service recieves all commands from the web application and then takes the appropriate action for the command. This is the connecting piece between the front end and backend of the whole simulation. 

## New Feature - Saving and Loading Simulations

### What does it do?
The feature that we added gives the user the ability to save a specific state of a simulation into a CSV file and then load in that simulation wheather it be in the same session or it be in the future. 

### Why is it signifigantly interesting?
Our save/load feature is interesting because it gives a user the ability to load other users simulationsinto their own, as well as send specific instances of simulations to other users and them be able to load them into their program. If a user encounters a certain instance of a simulation that they find interesting then they can save it and replay it over and over in order to investigate it further.

### How does it add to the existing work?
The feature adds to the existing project by creating a new class called Memento that does all of the heavy lifting of writing and reading from files, and adds 3 functions (getMemento, restore and save) into SimulationModel.cc code. 

#### Memento
The Memento class has 2 atttributes, a vector of Json object pointers and a fileName. The object pointers store the entities of the simulation associated with the Memento object. The fileName attribute holds the associated file's name. For functions, the Memento object has a constructor that sets the fileName attribute, a destructor that deletes all allocated pointers in the vector of Json object pointers, and there is a getter for fileNmae that is used in Simulation Model. The 3 most important functions in Memento are collectData, writeToCSV, and loadFromCSV. These functions names are descriptive enough so that you can guess what they do. The collectData function reads a list of all entitities and converts them from Entity objects into Json objects and stores them in the Json object pointer vector that Memento contains. The writeToCSV function uses the Json object pointer vector and writes all entities it contains to the CSV file associated with the current Memento object (If no current file exists it creates one). It writes a single entity/trip per line, this helpswith loading in from the CSV file. This function returns true if it succeeds and false if it fails. The third function is loadFromCSV and this function handles loading in the entity/trip objects from the CSV into the list of objects in the current Memento. This function returns the vector of loaded objects, and if the file is empty it returns an empty vector. All of these functions are not called internally and are used in SimulationModel.cc. The Memento object does most of the heavy lifting of loading and saving all of the data and is the intermediary between the data and save file.

#### SimulationModel
We added 3 functions to SimulationModel.cc, a save function, a restore function, and a getMemento function. The getMemento function is used in transit service to decide if there exists a memento for the file currently being loaded, if there does not then we exit, if there does it loads it. This sends a command to SimulationModel to load the specified file into the current simulation. this involves deleting the entities currently in the simulation and restoring the CSV's entities by calling loadFromCSV and using the vector it returns to create trips and entities accordingly. We also added a save function into SimulationModel.cc that simply calls collectData from Memento and then calls writeToCSV from memento.

#### Other Changes
Other changes that were made is that there were 2 fields added to the UI front end, a save simulation button and a file selection field to load in a specified file. Depending on the button that was pressed, 2 new commands are sent to the transit_service.cc and then the appropriate action is taken.

### Which design pattern used
We used the Memento design pattern that enables a program to save a specific state of itself inside a Memento object. We chose to use this design pattern because it was simple and suited our idea of saving a state of the program perfectly. Memento does not come out of the box able to restore any state and so we added that part of it to fit our problem exactly.

## How to use the Save / Load feature
First, you must have all save files in the /saves folder in order for this feature to work properly.

### Saving a file
Saving a file is quite simple, all you need to to is boot up the simulation and hit the save simulation button. Then your saved simulation CSV will be located in th /saves folder within the project directory.

### Loading a file
In order to load a file, you must first have the save you want to load within the /saves project directory. Next, you can boot up the simulation and go into the Choose File field, this will then prompt you to choose a file to load, if the file is valid and it exists within the saves folder then it will be loaded automatically into the simulation, if the file is not valid (if the file does not exist in /saves) then it will not be loaded and nothing will happen in the simulation.

## Sprint Retrospective
Our team organized our sprints into 2 week sprints, and divided tasks equally among members. We agree that the 2 week sprints were a good idea in thought however we did not do much during the first sprint so most of the tasks from that sprint got pushed into the next one, where we essentially did the entire project in the 2nd sprint. That is one thing that we did not do well was our actual work ethic during the first sprint. One of the things that went well during both of the sprints, but primarily the second one, is the fact that we assigned tasks instead of just letting it be a free for all. This improved overall organization of the 2 sprints as no one was sitting around wondering what to do next and someone always had another task to do.

## Links

### Docker

### Video Presentation

### UML