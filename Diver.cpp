#include "Diver.h"

// Name: Diver() - Default Constructor
// Desc: Used to build a new Diver
// Preconditions - Requires default values
// Postconditions - Creates a new Diver
Diver::Diver() {
	m_myName = "";
	m_numMaterials = 0;
}
// Name: Diver(name) - Overloaded constructor
// Desc - Used to build a new Diver
// Preconditions - Requires name
// Postconditions - Creates a new Diver
Diver::Diver(string name) {
	m_myName = name;
	m_numMaterials = 0;
}
// Name: GetName()
// Desc - Getter for Diver name
// Preconditions - Diver exists
// Postconditions - Returns the name of the Diver
string Diver::GetName() {
	return m_myName;
}
// Name: SetName(string)
// Desc - Allows the user to change the name of the Diver
// Preconditions - Diver exists
// Postconditions - Sets name of Diver
void Diver::SetName(string name) {
	m_myName = name;
}
// Name: DisplayMaterials()
// Desc - Displays a numbered list of all materials known by the Diver
// Preconditions - Diver exists
// Postconditions - Displays all materials
void Diver::DisplayMaterials() {
	for (int i = 0; i < PROJ2_SIZE; i++) {
		cout << i + 1 << "." << m_myMaterials[i].m_name << " " << m_myMaterials[i].m_quantity << endl;
	}
}

// Name: CheckMaterial(Material)
// Desc - Checks to see if the Diver has a material
// Preconditions - Diver already has materials
// Postconditions - Returns index of material if Diver has it else -1
int Diver::CheckMaterial(Material material) {
	for (int i = 0; i < PROJ2_SIZE; i++) {
		if (material.m_name == m_myMaterials[i].m_name) {
			return i;
		}
	}
	return -1;
}

// Name: AddMaterial(string)
// Desc - Inserts a material into the Diver's list of materials
// Preconditions - Diver exists
// Postconditions - Add a material to the Diver's m_myMaterials with a quantity of 0
void Diver::AddMaterial(Material material) {
	m_myMaterials[m_numMaterials] = material;
	m_numMaterials++;
}

// Name: IncrementQuantity(Material)
// Desc - Adds quantity to list of materials the diver knows
// Preconditions - Diver exists
// Postconditions - Increases quantity of material for diver
void Diver::IncrementQuantity(Material material) {
	int index = CheckMaterial(material);
	m_myMaterials[index].m_quantity++;
}

// Name: DecrementQuantity(Material)
// Desc - Reduces quantity from diver's inventory with true, if no quantity false
// Preconditions - Diver exists
// Postconditions - Reduces quantity of material for diver
bool Diver::DecrementQuantity(Material material) {
	int index = CheckMaterial(material);
	if (m_myMaterials[index].m_quantity > 0) {
		m_myMaterials[index].m_quantity--;
		return true;
	}

	return false;
}

// Name: CheckQuantity(Material)
// Desc - Checks to see if quantity of two merge materials are available
// Preconditions - Diver exists with materials
// Postconditions - Returns true if both are available (including two of same material)
bool Diver::CheckQuantity(Material materialOne, Material materialTwo) {
	int indexOne = CheckMaterial(materialOne);
	int indexTwo = CheckMaterial(materialTwo);

	//if both the materials are the same quantity must be at least 2
	if (indexOne == indexTwo) {
		if (m_myMaterials[indexOne].m_quantity > 1) {
			return true;
		}
		return false;
	}
	//if they are not the same quantity needs to be at least 1 for both
	if ((m_myMaterials[indexOne].m_quantity > 0) && (m_myMaterials[indexTwo].m_quantity > 0)) {
		return true;
	}
	//else there is not enough material
	return false;
}

// Name: GetMaterial(int)
// Desc - Given an index, returns the material at that index
// Preconditions - Diver already has materials
// Postconditions - The material object at that position
Material Diver::GetMaterial(int index) {
	return m_myMaterials[index];
}

// Name: CalcDepth(int)
// Desc - Checks the total depth of all unique materials known/built
// Preconditions - Diver already has materials
// Postconditions - Returns depth sum of all unique materials owned/built
int Diver::CalcDepth() {
	int depth = 0;
	for (int i = 0; i < PROJ2_SIZE; i++) {
		//only if there is one material can you add depth
		if (m_myMaterials[i].m_quantity > 0) {
			depth += m_myMaterials[i].m_depth;
		}
	}
	return depth;
}

