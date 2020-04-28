// Title: Game.cpp
//Author: Nicholas Perry
//Date: 2/24/2020
//Description: This is part of the Subnautica Project in CMSC 202 @ UMBC

#include "Game.h"

/* Name: Game() Default Constructor
// Desc - Builds a new game by:
// 1. Loads all materials and their recipes into m_materials
// 2. Asks user for their diver's name (store in m_myDiver as m_myName)
// 3. Copies all materials from m_materials into the diver's material array
// Preconditions - None
// Postconditions - m_materials is populated
*/
Game::Game() {
    string name;

    GameTitle();
    LoadMaterials();

    //copies m_materials to m_myMaterials in diver
    for (int i = 0; i < PROJ2_SIZE; i++) {
        m_myDiver.AddMaterial(m_materials[i]);
    }

    cout << "What is the name of your Diver? " << endl;
    getline(cin, name);
    m_myDiver.SetName(name);
}

//Default Constructor
// Name: LoadMaterials
// Desc - Loads each material into m_materials from file
// Preconditions - Requires file with valid material data
// Postconditions - m_material is populated with material structs
void Game::LoadMaterials() {
    ifstream input;
    input.open(PROJ2_DATA);
    string name;
    string type;
    string materialOne;
    string materialTwo;
    int depth;
    
    //input for a single line
    for (int i = 0; i < PROJ2_SIZE; i++) {
        getline(input, name, ',');
        m_materials[i].m_name = name;
        
        getline(input, type, ',');
        m_materials[i].m_type = type;

        getline(input, materialOne, ',');
        m_materials[i].m_material1 = materialOne;
        
        getline(input, materialTwo, ',');
        m_materials[i].m_material2 = materialTwo;
        
        input >> depth;
        m_materials[i].m_depth = depth;
        input.ignore(256, '\n');

        m_materials[i].m_quantity = 0;
    }
    input.close();  
}

// Name: StartGame()
// Desc - Manages the game itself including win conditions continually
//         calling the main menu 
// Preconditions - Player is placed in game
// Postconditions - Continually checks to see if player has entered 5
void Game::StartGame() {
    int menuOption = MainMenu();

    //the menu options loop
    while (menuOption != QUIT_VALUE && menuOption != WIN_VALUE) {
        if (menuOption == 1){
            DisplayMaterials();
        }
        else if (menuOption == 2) {
            SearchMaterials();
        }
        else if (menuOption == 3) {
            CombineMaterials();
        }
        else if (menuOption == 4) {
            CalcScore();
        }
        menuOption = MainMenu();
    }

    //if MainMenu returns that the user reached 1000
    if (menuOption == WIN_VALUE) {
        cout << "Congrats you won!" << endl;
    }
    cout << "Thanks for playing Subnautica" << endl;
}

// Name: DisplayMyMaterials()
// Desc - Displays the diver's materials
// Preconditions - Player's diver has materials
// Postconditions - Displays a numbered list of materials
void Game::DisplayMaterials() {
    m_myDiver.DisplayMaterials();
}

// Name: MainMenu()
// Desc - Displays and manages menu. Also checks win condition (diver reaches 1000 meters).
// Preconditions - Player has an Diver
// Postconditions - Returns number including exit
int Game::MainMenu() {
    int returnValue;
    
    //if depth the target depth was reached 
    if (m_myDiver.CalcDepth() >= MAX_DEPTH) {
        return WIN_VALUE;
    }

    //display menu and ask for a value
    cout << "What would you like to do? " << endl;
    cout << "1. Display your Diver's Materials" << endl;
    cout << "2. Search for Raw Materials" << endl;
    cout << "3. Attempt to Merge Materials" << endl;
    cout << "4. See Score" << endl;
    cout << "5. Quit" << endl;
    cin >> returnValue;

    //if the return value was not good
    while (returnValue < 1 || returnValue > 5) {
        cout << "What would you like to do? " << endl;
        cout << "1. Display your Diver's Materials" << endl;
        cout << "2. Search for Raw Materials" << endl;
        cout << "3. Attempt to Merge Materials" << endl;
        cout << "4. See Score" << endl;
        cout << "5. Quit" << endl;
        cin.clear();
        cin.ignore();
        cin >> returnValue;
    }

    return returnValue;
}

// Name: SearchMaterials()
// Desc - Attempts to search for raw materials (must be type "raw")
// Preconditions - Raw materials loaded
// Postconditions - Increases quantity in Diver's posession of materials
void Game::SearchMaterials() {
    //seeding has been done in the main file

    //generate a random number from 0 to 16 then prints and adds 1 quantity
    int randomNum = (rand() % MAX_SEARCH_VALUE + 1 - MIN_SEARCH_VALUE);
    cout << m_myDiver.GetMaterial(randomNum).m_name << " Found!" << endl;
    m_myDiver.IncrementQuantity(m_materials[randomNum]);
}

// Name: CombineMaterials()
// Desc - Attempts to combine known materials. If combined, quantity of material decreased
// Preconditions - Diver is populated with materials
// Postconditions - Increments quantity of item "made", decreases quantity of source items
void Game::CombineMaterials() {
    int combineList[COMBINE_LIST_LENGTH];

    int combinedValueOne;
    int combinedValueTwo;
    string materialOneName;
    string materialTwoName;
    
    int mergeIndex;
    int mergeValue;

    //
    for (int i = 0; i < COMBINE_LIST_LENGTH; i++) {
        //sets mergeValue
        RequestMaterial(mergeValue);
        //if the user prints the list rerun that index
        if (mergeValue == -1) {
            i--;
        }
        else {
            combineList[i] = mergeValue;
        }
    }

    //gets index for both choosen
    combinedValueOne = combineList[0] - 1;
    combinedValueTwo = combineList[1] - 1;

    //get the names of both
    materialOneName = m_materials[combinedValueOne].m_name;
    materialTwoName = m_materials[combinedValueTwo].m_name;

    //gets the index of both of the combined materials
    mergeIndex = SearchRecipes(materialOneName, materialTwoName);

    //print it all out and increment
    if (mergeIndex != -1) {
        cout << " to make " << m_materials[mergeIndex].m_name << "!" << endl;
        m_myDiver.IncrementQuantity(m_materials[mergeIndex]);
        cout << "Your diver has built " << m_materials[mergeIndex].m_name << endl;
    }
}

// Name: RequestMaterial()
// Desc - Requests material to use to merge
// Preconditions - Diver has materials to try and merge
// Postconditions - Returns integer of item selected by user
void Game::RequestMaterial(int& choice) {
    cout << "Which materials would you like to merge?\nTo list known materials enter -1" << endl;
    cin >> choice;

    //if choice value was not good
    while ((choice < 1 || choice > 50) && choice != -1) {
        cout << "Which materials would you like to merge?\nTo list known materials enter -1" << endl;
        cin.clear();
        cin.ignore();
        cin >> choice;
    }

    if (choice == -1) {
        DisplayMaterials();
    }
}

// Name: SearchRecipes()
// Desc - Searches recipes for two strings (name of item)
// Preconditions - m_materials is populated
// Postconditions - Returns int index of matching recipe
int Game::SearchRecipes(string materialOne, string materialTwo) {
    bool materialSlotOneMatch;
    bool materialSlotTwoMatch;

    bool isUnique;
    bool quantityZero;

    //gets the index in the array for the two sent materials 
    int materialOneIndex = GetMaterialIndex(materialOne);
    int materialTwoIndex = GetMaterialIndex(materialTwo);

    //makes sure you have at least one of each
    if (m_myDiver.CheckQuantity(m_materials[materialOneIndex], m_materials[materialTwoIndex])) {
        for (int i = 0; i < PROJ2_SIZE; i++) {
            //true if it is of unique type
            isUnique = m_materials[i].m_type == UNIQUE;
            //true if it is of quantity zero
            quantityZero = m_myDiver.GetMaterial(i).m_quantity == 0;
            
            //variables will be true if material one is in material1 slot and material two is in material2 slot
            materialSlotOneMatch = m_materials[i].m_material1 == materialOne;
            materialSlotTwoMatch = m_materials[i].m_material2 == materialTwo;

            //checks if they are true and decrements the needed materials 
            if (materialSlotOneMatch && materialSlotTwoMatch) {
                //checks for type unique and if so quantity one
                if (!isUnique || (isUnique && quantityZero)) {
                    m_myDiver.DecrementQuantity(m_materials[materialOneIndex]);
                    m_myDiver.DecrementQuantity(m_materials[materialTwoIndex]);
                    cout << m_materials[materialOneIndex].m_name << " combined with " << m_materials[materialTwoIndex].m_name;
                    return i;
                }
                else {
                    cout << "That item is unique and you alread have one!" << endl;
                }   
            }

            //if not checks material one to slot 2 and material two to slot 1
            materialSlotOneMatch = m_materials[i].m_material1 == materialTwo;
            materialSlotTwoMatch = m_materials[i].m_material2 == materialOne;

            //checks if the swap is true and decrements the needed materials 
            if (materialSlotOneMatch && materialSlotTwoMatch) {
                //checks for type unique and if so quantity one
                if (!isUnique || (isUnique && quantityZero)) {
                    m_myDiver.DecrementQuantity(m_materials[materialOneIndex]);
                    m_myDiver.DecrementQuantity(m_materials[materialTwoIndex]);
                    cout << m_materials[materialOneIndex].m_name << " combined with " << m_materials[materialTwoIndex].m_name;
                    return i;
                }
                else {
                    cout << "That item is unique and you alread have one!" << endl;
                }
            }
        }
    }
    else {
        cout << "Not enough material" << endl;
    }
    //all else fails, returns a -1
    return -1;
}

// Name: GetMaterialIndex()
// Desc - After being sent the m_name, returns the index
// Preconditions - m_materials is populated
// Postconditions - Returns int index of matching material, -1 by defualt
int Game::GetMaterialIndex(string name) {
    for (int i = 0; i < PROJ2_SIZE; i++) {
        if (m_materials[i].m_name == name) {
            return i;
        }
    }
    return -1;
}

// Name: CalcScore()
// Desc - Displays current score for Diver
// Preconditions - Diver is populated with materials
// Postconditions - Displays total
void Game::CalcScore() {
    cout << "***The Diver ***" << endl;
    cout << "The Great Diver " << m_myDiver.GetName() << endl;
    cout << "Maximum Depth: " << MAX_DEPTH << endl;
    cout << "Current Depth: " << m_myDiver.CalcDepth() << endl;
}
