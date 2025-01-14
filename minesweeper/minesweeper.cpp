#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <cmath>

using std::cout;
using std::vector;
using std::pair;
using std::endl;
using std::map;
using std::ifstream;
using std::string;
//*******************************************************************************************************************
//***********************************************BLOCKS CLASS*******************************************************
//*******************************************************************************************************************
class blocks
{
private:
	bool isBomb;
	bool isFlag;
	bool isRevealed;
	int bombsInProximity;
	int width = 25;
	int height = 16;
	int size = 32;

public:
	blocks()
	{
		isBomb = false;
		isFlag = false;
		isRevealed = false;
		bombsInProximity = 0;		
	}

	blocks(bool isBomb, bool isFlag, bool isRevealed, int bombsInProximity)
	{
		this->isBomb = isBomb;
		this->isFlag = isFlag;
		this->isRevealed = isRevealed;
		this->bombsInProximity = bombsInProximity;
	}

	void setIsBomb(bool isBomb)
	{
		this->isBomb = isBomb;
	}

	void setIsFlag(bool isFlag)
	{
		this->isFlag = isFlag;
	}

	void setisRevealed(bool isRevealed)
	{
		this->isRevealed = isRevealed;
	}

	void setBombsInProximity(int bombsInProximity)
	{
		this->bombsInProximity = bombsInProximity;
	}

	bool getIsBomb()
	{
		return isBomb;
	}

	bool getIsFlag()
	{
		return isFlag;
	}

	bool getisRevealed()
	{
		return isRevealed;
	}

	int getBombsInProximity()
	{
		return bombsInProximity;
	}

	int diceRoll16()
	{
		int diceRoll = rand() % 16;
		return diceRoll;
	}

	int diceRoll25()
	{
		int diceRoll = rand() % 25;
		return diceRoll;
	}
//*******************************************************************************************************************
//****************************************FILL VECT BLOCK CLASS FUNCTION******************************************
//*******************************************************************************************************************
	void fillVect(vector<vector<blocks>>& vect)
	{
		vect.resize(25, vector<blocks>(16));

		for (int i = 0; i < width; i++)		
			for (int j = 0; j < height; j++)
			vect[i][j] = blocks(false, false, false, 0);	
	}
//*******************************************************************************************************************
//****************************************FILL W/BOMBS BLOCK CLASS FUNCTION******************************************
//*******************************************************************************************************************
	void fillWithBombs(vector<vector<blocks>> & vect)
	{
		int count = 50;
		do
		{
			int x = diceRoll25();
			int y = diceRoll16();
			if (vect[x][y].getIsBomb() == false)
			{
				vect[x][y].setIsBomb(true);
				count--;
			}
		} while (count != 0);
	}
//*******************************************************************************************************************
//****************************************COUNT BOMBS BLOCK CLASS FUNCTION*******************************************
//*******************************************************************************************************************
	void countBombs(vector<vector<blocks>>& vect)
	{
		
		for (int i = 0; i < width; i++)
		{
			for (int j = 0; j < height; j++)
			{
				int count = 0;
	
				if (i - 1 < 0 || j - 1 < 0) {}										
				else if (vect[i - 1][j - 1].getIsBomb() == true)
				count++;					
							
				if (j - 1 < 0) {}
				else if (vect[i][j - 1].getIsBomb() == true)
				count++;
						
				if (i+1 > 24 || j-1 < 0) {}
				else if (vect[i + 1][j - 1].getIsBomb() == true)
				count++;
				
				if (i - 1 < 0) {}
				else if (vect[i - 1][j].getIsBomb() == true)
				count++;			
			
				if (i - 1 < 0|| j+1>15) {}
				else if (vect[i - 1][j + 1].getIsBomb() == true)					
				count++;					
				
				if (i + 1 >24) {}
				else if(vect[i + 1][j].getIsBomb() == true)
				count++;			

				if (j + 1 > 15) {}
				else if (vect[i][j + 1].getIsBomb() == true)
				count++;	
				
				if (i + 1 > 24 || j+1 > 15) {}
				else if (vect[i + 1][j + 1].getIsBomb() == true)
				count++;
				
				int temp;
				temp = vect[i][j].getBombsInProximity();
				temp += count;
				vect[i][j].setBombsInProximity(temp);
			}
		}
	}
//*******************************************************************************************************************
//****************************************REVEAL CELLS BLOCK CLASS FUNCTION******************************************
//*******************************************************************************************************************
	void revealCells(int x, int y, vector<vector<blocks>>& vect, bool& won, bool& lost, bool& playing)
	{
		if (vect[x][y].getIsFlag() == true)
			return;

		if (vect[x][y].getisRevealed() == true)
			return;

		if (vect[x][y].getIsBomb() == true)
		{
			vect[x][y].setisRevealed(true);
			lost = true;
			return;
		}

		if (vect[x][y].getBombsInProximity() == 0)
		{
			vect[x][y].setisRevealed(true);

			if (x - 1 < 0 || y - 1 < 0) {}
			else
			revealCells(x - 1, y - 1, vect, won, lost, playing);

			if (y - 1 < 0) {}
			else
			revealCells(x, y - 1, vect, won, lost, playing);

			if (x + 1 > 24 || y - 1 < 0) {}
			else
			revealCells(x + 1, y - 1, vect, won, lost, playing);

			if (x - 1 < 0) {}
			else
			revealCells(x - 1, y, vect, won, lost, playing);

			if (x - 1 < 0 || y + 1 > 15) {}
			else
			revealCells(x - 1, y + 1, vect, won, lost, playing);

			if (x + 1 > 24) {}
			else
			revealCells(x + 1, y, vect, won, lost, playing);

			if (y + 1 > 15) {}
			else
			revealCells(x, y + 1, vect, won, lost, playing);

			if (x + 1 > 24 || y + 1 > 15) {}
			else
			revealCells(x + 1, y + 1, vect, won, lost, playing);
		}

		vect[x][y].setisRevealed(true);

		checkWin(vect, won);
	}
//*******************************************************************************************************************
//****************************************CHECK WIN BLOCK CLASS FUNCTION******************************************
//*******************************************************************************************************************		
		void checkWin(vector<vector<blocks>>& vect, bool &won)
		{
			// check win
			for (int i = 0; i < width; i++)			
				for (int j = 0; j < height; j++)
				{
					if (vect[i][j].getIsBomb() == true && vect[i][j].getIsFlag() == true)
					continue;
					if (vect[i][j].getisRevealed() == false)
					return;
					if (vect[i][j].getisRevealed() == true && vect[i][j].getIsBomb() == true)
					return;
				}		
			won = true;
		}
//*******************************************************************************************************************
//****************************************TEST BLOCK CLASS FUNCTION**************************************************
//*******************************************************************************************************************		
	void test(vector<vector<blocks>>& vect, string fileName) 
	{
		ifstream files;
		files.open("boards/"+fileName);
		char Number;

		fillVect(vect);

			if (files.fail())
			{
				throw std::runtime_error("An error has occurred. File not loaded");
				exit(1);
			}
			else
			{
				if (files.is_open())
				{
					for (int i = 0; i < height; i++)					
						for (int j = 0; j < width; j++)
						{
							files >> Number;
							
							if (Number == '1')
							vect[j][i].setIsBomb(true);
							else
							vect[j][i].setIsBomb(false);
						}					
				}
				files.close();
			}
	countBombs(vect);
	}
};

void resetGame(vector<vector<blocks>>& vect, bool& won, bool& lost, bool& playing, bool &debugger, int &flagCount);

int main()
{
//*******************************************************************************************************************
//***********************************************MAIN VARIABLES******************************************************
//*******************************************************************************************************************
	blocks blocksOBJ;
//Declare Variables 
	bool won;
	bool lost;
	bool playing = false;
	bool debugger = false;
	vector<vector<blocks>> a;
	int flagCount;
	int width = 25;
	int height = 16;
	int size = 32;
//*******************************************************************************************************************
//***********************************************TEXTURES AND SPRITES************************************************
//*******************************************************************************************************************
//Declaring Textures
	sf::Texture forground;
	sf::Texture background;
	sf::Texture bombLayer;
	sf::Texture oneLayer;
	sf::Texture twoLayer;
	sf::Texture threeLayer;
	sf::Texture fourLayer;
	sf::Texture FiveLayer;
	sf::Texture sixLayer;
	sf::Texture sevenLayer;
	sf::Texture eightLayer;
	sf::Texture debugLayer;
	sf::Texture testOneLayer;
	sf::Texture testTwoLayer;
	sf::Texture testThreeLayer;
	sf::Texture winLayer;
	sf::Texture loseLayer;
	sf::Texture stillPlayingLayer;
	sf::Texture flagLayer;
	sf::Texture hexZeroLayer;
	sf::Texture hexOneLayer;
	sf::Texture hexTwoLayer;
	sf::Texture hexThreeLayer;
	sf::Texture hexFourLayer;
	sf::Texture hexFiveLayer;
	sf::Texture hexSixLayer;
	sf::Texture hexSevenLayer;
	sf::Texture hexEightLayer;
	sf::Texture hexNineLayer;
	sf::Texture hexNegLayer;	
//Loading Hex digits
	hexZeroLayer.	loadFromFile("images/digits.png", sf::IntRect(0*21, 0, 21, 32));
	hexOneLayer.	loadFromFile("images/digits.png", sf::IntRect(1*21, 0, 21, 32));
	hexTwoLayer.	loadFromFile("images/digits.png", sf::IntRect(2*21, 0, 21, 32));
	hexThreeLayer.	loadFromFile("images/digits.png", sf::IntRect(3*21, 0, 21, 32));
	hexFourLayer.	loadFromFile("images/digits.png", sf::IntRect(4*21, 0, 21, 32));
	hexFiveLayer.	loadFromFile("images/digits.png", sf::IntRect(5*21, 0, 21, 32));
	hexSixLayer.	loadFromFile("images/digits.png", sf::IntRect(6*21, 0, 21, 32));
	hexSevenLayer.	loadFromFile("images/digits.png", sf::IntRect(7*21, 0, 21, 32));
	hexEightLayer.	loadFromFile("images/digits.png", sf::IntRect(8*21, 0, 21, 32));
	hexNineLayer.	loadFromFile("images/digits.png", sf::IntRect(9*21, 0, 21, 32));
	hexNegLayer.	loadFromFile("images/digits.png", sf::IntRect(10*21, 0, 21, 32));
//Loading Texture Files
	forground.		  loadFromFile("images/tile_hidden.png");
	background.		  loadFromFile("images/tile_revealed.png");
	bombLayer.		  loadFromFile("images/mine.png");
	oneLayer.		  loadFromFile("images/number_1.png");
	twoLayer.		  loadFromFile("images/number_2.png");
	threeLayer.		  loadFromFile("images/number_3.png");
	fourLayer.		  loadFromFile("images/number_4.png");
	FiveLayer.		  loadFromFile("images/number_5.png");
	sixLayer.		  loadFromFile("images/number_6.png");
	sevenLayer.		  loadFromFile("images/number_7.png");
	eightLayer.		  loadFromFile("images/number_8.png");
	debugLayer.		  loadFromFile("images/debug.png");
	testOneLayer.	  loadFromFile("images/test_1.png");
	testTwoLayer.	  loadFromFile("images/test_2.png");
	testThreeLayer.	  loadFromFile("images/test_3.png");
	winLayer.		  loadFromFile("images/face_win.png");
	loseLayer.		  loadFromFile("images/face_lose.png");
	stillPlayingLayer.loadFromFile("images/face_happy.png");
	flagLayer.		  loadFromFile("images/flag.png");
//Declaring Sprites
	sf::Sprite hiddenTile;
	sf::Sprite revealedTile;
	sf::Sprite mine;
	sf::Sprite one;
	sf::Sprite two;
	sf::Sprite three;
	sf::Sprite four;
	sf::Sprite five;
	sf::Sprite six;
	sf::Sprite seven;
	sf::Sprite eight;
	sf::Sprite debugButton;
	sf::Sprite testOneButton;
	sf::Sprite testTwoButton;
	sf::Sprite testThreeButton;
	sf::Sprite win;
	sf::Sprite lose;
	sf::Sprite stillPlaying;
	sf::Sprite flag;
	sf::Sprite hexZero;
	sf::Sprite hexOne;
	sf::Sprite hexTwo;
	sf::Sprite hexThree;
	sf::Sprite hexFour;
	sf::Sprite hexFive;
	sf::Sprite hexSix;
	sf::Sprite hexSeven;
	sf::Sprite hexEight;
	sf::Sprite hexNine;
	sf::Sprite hexNeg;
//Loading Sprites with textures
	hiddenTile.		setTexture(forground);
	revealedTile.	setTexture(background);
	mine.			setTexture(bombLayer);
	one.			setTexture(oneLayer);
	two.			setTexture(twoLayer);
	three.			setTexture(threeLayer);
	four.			setTexture(fourLayer);
	five.			setTexture(FiveLayer);
	six.			setTexture(sixLayer);
	seven.			setTexture(sevenLayer);
	eight.			setTexture(eightLayer);
	debugButton.	setTexture(debugLayer);
	testOneButton.	setTexture(testOneLayer);
	testTwoButton.	setTexture(testTwoLayer);
	testThreeButton.setTexture(testThreeLayer);
	win.			setTexture(winLayer);
	lose.			setTexture(loseLayer);
	stillPlaying.	setTexture(stillPlayingLayer);
	flag.			setTexture(flagLayer);
	hexZero.		setTexture(hexZeroLayer);
	hexOne.			setTexture(hexOneLayer);
	hexTwo.			setTexture(hexTwoLayer);
	hexThree.		setTexture(hexThreeLayer);
	hexFour.		setTexture(hexFourLayer);
	hexFive.		setTexture(hexFiveLayer);
	hexSix.			setTexture(hexSixLayer);
	hexSeven.		setTexture(hexSevenLayer);
	hexEight.		setTexture(hexEightLayer);
	hexNine.		setTexture(hexNineLayer);
	hexNeg.			setTexture(hexNegLayer);

	sf::RenderWindow window(sf::VideoMode(800, 600), "Minesweeper!");
//*******************************************************************************************************************
//***********************************************SET GAME CONDITIONS*************************************************
//*******************************************************************************************************************
	if (playing == false)
	{	
		flagCount = 50;
		blocksOBJ.fillVect(a);
		blocksOBJ.fillWithBombs(a);
		blocksOBJ.countBombs(a);
		won = false;
		lost = false;
		playing = true;
		debugger = false;
	}
//*******************************************************************************************************************
//***********************************************WINDOW START********************************************************
//*******************************************************************************************************************
		while (window.isOpen())
		{
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
//*******************************************************************************************************************
//***********************************************RIGHT CLICK FUNCTIONALITY*******************************************
//*******************************************************************************************************************
				if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && lost==false && won == false)
				{
					sf::Vector2i mousePos = sf::Mouse::getPosition(window);
					int x = (int)mousePos.x / size;
					int y = (int)mousePos.y / size;

					if (y >= 0 && y <= 15 && x >= 0 && x <= 24)
					{					
						if (a[x][y].getIsFlag() == false && a[x][y].getisRevealed() == false)
						{
							a[x][y].setIsFlag(true);
							flagCount--;
						}
						else if (a[x][y].getIsFlag() == true && a[x][y].getisRevealed() == false)
						{
							a[x][y].setIsFlag(false);
							flagCount++;
						}
					}
					blocksOBJ.checkWin(a, won);
					
				}
//*******************************************************************************************************************
//***********************************************LEFT CLICK FUNCTIONALITY********************************************
//*******************************************************************************************************************
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					sf::Vector2i mousePos = sf::Mouse::getPosition(window);

					int x = mousePos.x;
					int y = mousePos.y;
					
					if (y/size >= 0 && y/size <= 15 && x/size >= 0 && x/size <= 24 && lost == false)
					{
						blocksOBJ.revealCells(x/size, y/size, a, won, lost, playing);
					}
					//reset game
					if ((x  >= 11.5 * size) && (x <= 13.5 * size) && (y >= 16* size) && (y <= 18 * size))
					{
						flagCount = 50;
						blocksOBJ.fillVect(a);
						blocksOBJ.fillWithBombs(a);
						blocksOBJ.countBombs(a);
						won = false;
						lost = false;
						debugger = false;
					}
//*******************************************************************************************************************
//***********************************************DEBUG BUTTON CLICK**************************************************
//*******************************************************************************************************************				
					if (((x  >= 15.5 * size) && (x  <= 17.5 * size) && (y>= 16 * size) && (y <= 18 * size)))
					{						
						if (debugger == false)
						{
							for (int i = 0; i < width; i++)
								for (int j = 0; j < height; j++)
								{

									if (a[i][j].getIsBomb() == true)
										a[i][j].setisRevealed(true);
									
								}
							debugger = true;
							
						}					
						else if (debugger==true)
						{
							for (int i = 0; i < width; i++)
								for (int j = 0; j < height; j++)
								{
									if (a[i][j].getIsBomb() == true)
										a[i][j].setisRevealed(false);
								}
							debugger = false;
							
						}
					}
//*******************************************************************************************************************
//***********************************************Test 1-3************************************************************
//*******************************************************************************************************************
					//test one
					if ((x >= 17.5 * size) && (x <= 19.5 * size) && (y >= 16 * size) && (y <= 18 * size))
					{
					resetGame(a, won, lost, playing, debugger, flagCount);
					blocksOBJ.test(a, "testboard1.brd");
					}
					//test two
					if ((x >= 19.5 * size) && (x <= 21.5 * size) && (y >= 16 * size) && (y <= 18 * size))
					{
					resetGame(a, won, lost, playing, debugger, flagCount);
					blocksOBJ.test(a, "testboard2.brd");
					}
					//test three
					if ((x >= 21.5 * size) && (x <= 23.5 * size) && (y >= 16 * size) && (y <= 18 * size))		
					{
					resetGame(a, won, lost, playing, debugger, flagCount);
					blocksOBJ.test(a, "testboard3.brd");
					}
				}
			}
			window.clear();
//*******************************************************************************************************************
//***********************************************Bombs In Proximity Draw*********************************************
//*******************************************************************************************************************			
				for (float i = 0.0; i < 25; i++)
				{
					for (float j = 0.0; j < 16; j++)
					{
						hiddenTile.  setPosition(sf::Vector2f(i * size, j * size));
						revealedTile.setPosition(sf::Vector2f(i * size, j * size));
						one.		 setPosition(sf::Vector2f(i * size, j * size));
						two.		 setPosition(sf::Vector2f(i * size, j * size));
						three.		 setPosition(sf::Vector2f(i * size, j * size));
						four.		 setPosition(sf::Vector2f(i * size, j * size));
						five.		 setPosition(sf::Vector2f(i * size, j * size));
						six.		 setPosition(sf::Vector2f(i * size, j * size));
						seven.		 setPosition(sf::Vector2f(i * size, j * size));
						eight.		 setPosition(sf::Vector2f(i * size, j * size));
						flag.		 setPosition(sf::Vector2f(i * size, j * size));
						mine.		 setPosition(sf::Vector2f(i * size, j * size));

						if (a[i][j].getisRevealed() == true && a[i][j].getIsFlag() == false)
						{
							if (a[i][j].getBombsInProximity() == 1 && a[i][j].getIsBomb() == false)
							{
								window.draw(revealedTile);
								window.draw(one);
							}

							else if (a[i][j].getBombsInProximity() == 2 && a[i][j].getIsBomb() == false)
							{
								window.draw(revealedTile);
								window.draw(two);
							}

							else if (a[i][j].getBombsInProximity() == 3 && a[i][j].getIsBomb() == false)
							{
								window.draw(revealedTile);
								window.draw(three);
							}

							else if (a[i][j].getBombsInProximity() == 4 && a[i][j].getIsBomb() == false)
							{
								window.draw(revealedTile);
								window.draw(four);
							}

							else if (a[i][j].getBombsInProximity() == 5 && a[i][j].getIsBomb() == false)
							{
								window.draw(revealedTile);
								window.draw(five);
							}

							else if (a[i][j].getBombsInProximity() == 6 && a[i][j].getIsBomb() == false)
							{
								window.draw(revealedTile);
								window.draw(six);
							}

							else if (a[i][j].getBombsInProximity() == 7 && a[i][j].getIsBomb() == false)
							{
								window.draw(revealedTile);
								window.draw(seven);
							}

							else if (a[i][j].getBombsInProximity() == 8 && a[i][j].getIsBomb() == false)
							{
								window.draw(revealedTile);
								window.draw(eight);
							}
							else
							{
								window.draw(revealedTile);
							}
							if (a[i][j].getIsBomb() == true)
							{
								window.draw(mine);
							}
						}
//*******************************************************************************************************************
//***********************************************Flag Draw***********************************************************
//*******************************************************************************************************************
						else if (a[i][j].getisRevealed() == false)
						{
							window.draw(hiddenTile);
						}

						if (a[i][j].getIsFlag() == true)
						{
							window.draw(hiddenTile);
							window.draw(flag);
						}
//*******************************************************************************************************************
//***********************************************Reset Button W/Smiley faces*****************************************
//*******************************************************************************************************************
						if (won == true)
						{
							win.setPosition(sf::Vector2f(11.5 * size, 16 * size));
							window.draw(win);
						}
						else if (lost == true)
						{
							lose.setPosition(sf::Vector2f(11.5 * size, 16 * size));
							if (a[i][j].getIsBomb() == true)
							{
								window.draw(revealedTile);
								window.draw(mine);
							}
							window.draw(lose);
						}
						else if (playing == true)
						{
							stillPlaying.setPosition(sf::Vector2f(11.5 * size, 16 * size));
							window.draw(stillPlaying);
						}
					}
				}
//*******************************************************************************************************************
//***********************************************DEBUG AND TEST BUTTONS**********************************************
//*******************************************************************************************************************
				debugButton.setPosition(sf::Vector2f(15.5 * size, 16 * size));
				window.draw(debugButton);

				testOneButton.setPosition(sf::Vector2f(17.5 * size, 16 * size));
				window.draw(testOneButton);

				testTwoButton.setPosition(sf::Vector2f(19.5 * size, 16 * size));
				window.draw(testTwoButton);

				testThreeButton.setPosition(sf::Vector2f(21.5 * size, 16 * size));
				window.draw(testThreeButton);
//*******************************************************************************************************************
//***********************************************Counter variables***************************************************
//*******************************************************************************************************************
				int tens=abs(flagCount/10);
				int ones =abs(flagCount%10);
//*******************************************************************************************************************
//*************************************************Middle digit******************************************************
//*******************************************************************************************************************
				if (tens == 9)
				{
					hexNine.setPosition(sf::Vector2f(1 * 21, 16 * size));
					window.draw(hexNine);
				}
				else if (tens == 8)
				{
					hexEight.setPosition(sf::Vector2f(1 * 21, 16 * size));
					window.draw(hexEight);
				}
				else if (tens == 7)
				{
					hexSeven.setPosition(sf::Vector2f(1 * 21, 16 * size));
					window.draw(hexSeven);
				}
				else if (tens == 6)
				{
					hexSix.setPosition(sf::Vector2f(1 * 21, 16 * size));
					window.draw(hexSix);
				}
				else if (tens == 5)
				{
					hexFive.setPosition(sf::Vector2f(1 * 21, 16 * size));
					window.draw(hexFive);
				}
				else if (tens == 4)
				{
					hexFour.setPosition(sf::Vector2f(1 * 21, 16 * size));
					window.draw(hexFour);
				}
				else if (tens == 3)
				{
					hexThree.setPosition(sf::Vector2f(1 * 21, 16 * size));
					window.draw(hexThree);
				}
				else if (tens == 2)
				{
					hexTwo.setPosition(sf::Vector2f(1 * 21, 16 * size));
					window.draw(hexTwo);
				}
				else if (tens == 1) 
				{
					hexOne.setPosition(sf::Vector2f(1 * 21, 16 * size));
					window.draw(hexOne);
				}
				else if (tens == 0)
				{
					hexZero.setPosition(sf::Vector2f(1*21, 16 * size));
					window.draw(hexZero);
				}
				else
				{
					hexZero.setPosition(sf::Vector2f(1 * 21, 16 * size));
					window.draw(hexZero);
				}
//******************************************************************************************************************
//************************************************Right Most digit**************************************************
//******************************************************************************************************************
				if (ones == 9)
				{
					hexNine.setPosition(sf::Vector2f(2 * 21, 16 * size));
					window.draw(hexNine);
				}
				else if (ones == 8)
				{
					hexEight.setPosition(sf::Vector2f(2 * 21, 16 * size));
					window.draw(hexEight);
				}
				else if (ones == 7)
				{
					hexSeven.setPosition(sf::Vector2f(2 * 21, 16 * size));
					window.draw(hexSeven);
				}
				else if (ones == 6)
				{
					hexSix.setPosition(sf::Vector2f(2 * 21, 16 * size));
					window.draw(hexSix);
				}
				else if (ones == 5)
				{
					hexFive.setPosition(sf::Vector2f(2 * 21, 16 * size));
					window.draw(hexFive);
				}
				else if (ones == 4)
				{
					hexFour.setPosition(sf::Vector2f(2 * 21, 16 * size));
					window.draw(hexFour);
				}
				else if (ones == 3)
				{
					hexThree.setPosition(sf::Vector2f(2 * 21, 16 * size));
					window.draw(hexThree);
				}
				else if (ones == 2)
				{
					hexTwo.setPosition(sf::Vector2f(2 * 21, 16 * size));
					window.draw(hexTwo);
				}
				else if (ones == 1)
				{
					hexOne.setPosition(sf::Vector2f(2 * 21, 16 * size));
					window.draw(hexOne);
				}
				else if (ones == 0)
				{
					hexZero.setPosition(sf::Vector2f(2*21, 16 * size));
					window.draw(hexZero);
				}
				else
				{
					hexZero.setPosition(sf::Vector2f(2*21, 16 * size));
					window.draw(hexZero);
				}
//******************************************************************************************************************
//************************************************Left Most digit***************************************************
//******************************************************************************************************************
				if (flagCount < 0)
				{
					hexNeg.setPosition(sf::Vector2f(0 * 21, 16 * size));
					window.draw(hexNeg);
				}
				else
				{
					hexZero.setPosition(sf::Vector2f(0 * 21, 16 * size));
					window.draw(hexZero);
				}
				window.display();			
		}
return 0;
}
//*******************************************************************************************************************
//***********************************************RESET GAME FUNCTION*************************************************
//*******************************************************************************************************************
void resetGame(vector<vector<blocks>>& vect, bool& won, bool& lost, bool& playing,bool &debugger, int &flagCount)
{
	flagCount = 50;
	won = false;
	lost = false;
	playing = true;
	debugger = false;
	vect.clear();
}




