#include <iostream>
#include <SFML/Graphics.hpp>
#include <random>
#include <SFML/Audio.hpp>
#include <fstream>
#include <iostream>


auto textGenerate(const std::string& str, sf::Font& font, int charSize,
                  sf::Color color, sf::Text::Style style)-> sf::Text{
    auto text = sf::Text();
    text.setString(str);
    text.setFont(font);
    text.setCharacterSize(charSize);
    text.setFillColor(color);
    text.setStyle(style);

    return text;
}

//overloading an existing function
auto textGenerate(sf::Text& text, sf::Color color, sf::Text::Style style, int outlineThickness = 0,
                  sf::Color outlineColor = sf::Color::Black)-> sf::Text{
    text.setFillColor(color);
    text.setStyle(style);
    text.setOutlineThickness(outlineThickness);
    text.setOutlineColor(outlineColor);
    return text;
}

auto loadTexture(const std::string& location){
    auto texture = sf::Texture();
    texture.loadFromFile(location);
    return texture;
}

auto spriteGenerate(const sf::Texture& texture,
                    int originX=0, int originY=0, int posX=0, int posY=0, int rot=0)->sf::Sprite{
    auto sprite = sf::Sprite();
    sprite.setTexture(texture);
    sprite.setOrigin(originX,originY);
    sprite.setPosition(posX,posY);
    sprite.setRotation(rot);
    return sprite;
}


//tried to use but gives an error
auto baseDrawMainMenu(sf::RenderWindow window, sf::Sprite mainMenuSprite,sf::Sprite movingLineGradientSprite,
                      std::vector<sf::Text> movingWords, sf::Text newGameText, sf::Text settingsText, sf::Text infoText,
                      sf::Text quitText){
    //drawing visuals
    window.draw(mainMenuSprite);
    window.draw(movingLineGradientSprite);
    //drawing running stripe LOGO words
    for(const auto& mowingWord : movingWords){
        window.draw(mowingWord);
    }
    //Drawing main menu buttons
    window.draw(newGameText);
    window.draw(settingsText);
    window.draw(infoText);
    window.draw(quitText);
}




auto randomNumGenerator(int min, int max){
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> distribution(min,max);

    return distribution(gen);
}

auto main()->int {
    //Reading from a file
    std::ofstream scoreFile("../Scores.txt");

    std::ifstream file("../worddoc.txt");
    if(!file.is_open()){
        std::cout << "falied to open";
    }
    auto fileWord = std::string();

    //FOR NOW USING VECTOR OF WORDS
    auto words = std::vector<std::string>{"MONKEY"};
    while(file >> fileWord){
        words.push_back(fileWord);
    }


    //Everyone use it! It does it`s job.
    std::random_device rd;
    std::mt19937 g(rd());//Random element to swap words in range by switching their current indexed position to random ones.
    std::shuffle(words.begin(), words.end(), g);

    for(auto el : words){
        std::cout << el << "\n";
    }
    file.close();




    auto infoDescr = std::string("SHORTCUTS:"
                                 "\n\nESC - PAUSE / GO OUT OF THE WINDOW;"
                                 "\nCTRL + R - QUICK GAME RESTART;"
                                 "\nCTRL + Q - QUICK GAME QUIT;"
                                 "\nCTRL + P - QUICK PAUSE;"
                                 "\nCTRL + H - secret feature;");

    auto gameOverDescr = std::string("!GAME OVER!\n"
                                     "YOU HAVE MISSED 10 POSSIBLE WORDS!\n"
                                     "PRESS NEW GAME TO TRY AGAIN!");

    auto easyModeDescr = std::string("Speed of words: Slow"
                                     "\nSize of words: Normal"
                                     "\nNumber of words per spawn: 1-2");

    auto mediumModeDescr = std::string("Speed of words: Doable"
                                     "\nSize of words: Variative"
                                     "\nNumber of words per spawn: 1-3");

    auto hardModeDescr = std::string("Speed of words: Try2Survive"
                                     "\nSize of words: Variative"
                                     "\nNumber of words per spawn: 1-4");

    auto scoresList = std::string();




    //Vector of dots


    double speedOfWords = 1;
    auto speedOfStars = 2;
    auto speedOfMovingWords = 4;

    auto prevSpeedOfWords = speedOfWords;
    auto prevsSpeedOfStars = speedOfStars;

    int minYRand = 0;
    int maxYRand = 0;
    int minXRand = -80;
    int maxXRand = -40;
    int staticXSpawn = 200;
    int maxNumPerPos = 1;

    int minWordSize = 20;
    int maxWordSize = 20;


    //RANDDOM WORDS PER COLUMN AMOUNT RANGE VALUES
    auto minWordRand = 1;
    auto maxWordRand = 1;

    auto prevMinWordRand = minWordRand;
    auto prevMaxWordRand = maxWordRand;

    //CREATING VARIABLE WHICH WILL DEFINE A GAP BETWEEN EVERY SPAWN
    int spawnRate = 2;



    auto windowWidth = 1260;
    auto windowHeight = 840;

    auto window50p = (50 * windowWidth)/100;
    auto window80p = (80 * windowWidth)/100;

    auto window = sf::RenderWindow(
            sf::VideoMode(windowWidth, windowHeight),
            "Monkey Typer",
            sf::Style::Default
            );

    //SETTING A FRAMERATE
    window.setFramerateLimit(60);

    sf::ContextSettings(0,0,8);

    //auto word = sf::Text();
    auto font = sf::Font();
    auto vcrFont = sf::Font();
    //fonts to choose
    auto font1 = sf::Font();
    auto font2 = sf::Font();
    auto font3 = sf::Font();
    auto font4 = sf::Font();

    vcrFont.loadFromFile("../VCR_OSD_MONO.ttf");
    font.loadFromFile("../pixelated.ttf");
    font1.loadFromFile("../DotsFont.TTF");
    font2.loadFromFile("../OutlinedFont.TTF");
    font3.loadFromFile("../NIKEA.otf");
    font4.loadFromFile("../pixelated.ttf");

    auto leftSideGradientTexture = loadTexture("../SideGradient.png");
    auto leftSideGradientSprite = spriteGenerate(leftSideGradientTexture,0,0,-10,0);
    auto rightSideGradientSprite = spriteGenerate(leftSideGradientTexture,
                                                  0,0,windowWidth,640,180);

    auto mainMenuTexture = loadTexture("../MainMenuImg.png");
    sf::Sprite mainMenuSprite = spriteGenerate(mainMenuTexture);

    auto movingLineGradientTexture = loadTexture("../MovingLineGradient.png");
    sf::Sprite movingLineGradientSprite = spriteGenerate(movingLineGradientTexture,0,0,0,10);

    auto bigMenuTexture = loadTexture("../BigMenuImg.png");
    sf::Sprite bigMenuSprite = spriteGenerate(bigMenuTexture);

    auto statsBoxTexture = loadTexture("../StatsBox.png");
    sf::Sprite statsBoxSprite = spriteGenerate(statsBoxTexture,0,0,0,windowHeight-180);

    auto basicGuyTexture = loadTexture("../BasicGuy.png");
    sf::Sprite basicGuySprite = spriteGenerate(basicGuyTexture,0,0,800,600);

    auto thumbsUpTexture = loadTexture("../ThumbsUp.png");
    sf::Sprite thumbsUpSprite = spriteGenerate(thumbsUpTexture,0,0,800,600);

    auto thumbsDownTexture = loadTexture("../ThumbsDown.png");
    sf::Sprite thumbsDownSprite = spriteGenerate(thumbsDownTexture,0,0,800,590);

    auto hatTexture = loadTexture("../SecretHat.png");
    auto hatSprite = spriteGenerate(hatTexture,0,0,800,590);

    auto separatorTexture = loadTexture("../Separator.png");
    auto separatorSprite = spriteGenerate(separatorTexture,0,0,-55,0);




    sf::Text word = textGenerate("MONKEY", font, 15,
                                 sf::Color::White, sf::Text::Style::Regular);

    //STATIC TEXT:
    auto inputText = word;
    inputText.setString("Input: ");
    inputText.setOrigin(0,0);

    auto correctText = word;
    correctText.setString("Correct: ");
    correctText.setOrigin(0,0);

    auto incorrectText = word;
    incorrectText.setString("Incorrect: ");
    correctText.setOrigin(0,0);

    auto scoreStaticText = textGenerate("Your score:", font,23,sf::Color::White,sf::Text::Style::Bold);;
    correctText.setOrigin(0,0);
    scoreStaticText.setOutlineThickness(1);
    scoreStaticText.setOutlineColor(sf::Color::Red);
    scoreStaticText.setOrigin(0,0);
    scoreStaticText.setPosition(460,480);

    auto nameStaticText = textGenerate("Enter yourn name:", font,23,sf::Color::White,sf::Text::Style::Bold);;
    nameStaticText.setOrigin(0,0);
    nameStaticText.setOutlineThickness(1);
    nameStaticText.setOutlineColor(sf::Color::Red);
    nameStaticText.setOrigin(0,0);
    nameStaticText.setPosition(460,520);

    sf::Text infoDescrText = textGenerate(infoDescr,font,25,sf::Color::White,sf::Text::Style::Regular);
    infoDescrText.setOrigin(0,0);
    infoDescrText.setPosition(500,250);

    sf::Text gameOverDescrText = textGenerate(gameOverDescr,font,23,sf::Color::White, sf::Text::Style::Regular);
    gameOverDescrText.setOrigin(0,0);
    gameOverDescrText.setOutlineThickness(1);
    gameOverDescrText.setOutlineColor(sf::Color(255,131,122));
    gameOverDescrText.setPosition(460,340);



    sf::Text easyModeDescrText = textGenerate(easyModeDescr,font,15,sf::Color::White,sf::Text::Style::Regular);
    easyModeDescrText.setOutlineThickness(1);
    easyModeDescrText.setOutlineColor(sf::Color::Green);
    easyModeDescrText.setPosition(800,340);

    sf::Text mediumModeDescrText = textGenerate(mediumModeDescr,font,15,sf::Color::White,sf::Text::Style::Regular);
    mediumModeDescrText.setOutlineThickness(1);
    mediumModeDescrText.setOutlineColor(sf::Color::Yellow);
    mediumModeDescrText.setPosition(800,340);

    sf::Text hardModeDescrText = textGenerate(hardModeDescr,font,15,sf::Color::White,sf::Text::Style::Regular);
    hardModeDescrText.setOutlineThickness(1);
    hardModeDescrText.setOutlineColor(sf::Color::Red);
    hardModeDescrText.setPosition(800,340);

    sf::Text scoresListText = textGenerate(scoresList,font,25,sf::Color::White,sf::Text::Style::Bold);
    scoresListText.setOutlineThickness(1);
    scoresListText.setOutlineColor(sf::Color::Green);
    scoresListText.setPosition(500,250);

    sf::Text missedText = word;
    missedText.setString("Missed: ");
    inputText.setOrigin(0,0);

    sf::Text timeText = word;
    timeText.setString("Time: ");
    timeText.setCharacterSize(25);
    timeText.setOrigin(0,0);

    sf::Text result = word;\
    result.setString("RESULT:");
    result.setCharacterSize(35);
    result.setOrigin(0,0);


    //USER INPUT TEXT: (and variables)
    auto correctCounter = 0;
    auto incorrectCounter = 0;
    auto missedCounter = 0;

    auto userText = word;
    userText.setString("");
    userText.setOrigin(0,0);

    auto userCorrect = word;
    userCorrect.setString("");
    userCorrect.setOrigin(0,0);

    auto userIncorrect = word;
    userIncorrect.setString("");
    userIncorrect.setOrigin(0,0);

    auto userMissedText = word;
    userMissedText.setString("");
    inputText.setOrigin(0,0);

    auto currentTimeText = word;
    currentTimeText.setString("");
    currentTimeText.setCharacterSize(25);
    currentTimeText.setOrigin(0,0);

    auto userScore = word;
    userScore.setString("");
    userScore.setOutlineThickness(1);
    userScore.setOutlineColor(sf::Color::Red);
    userScore.setCharacterSize(23);
    userScore.setOrigin(0,0);
    userScore.setPosition(scoreStaticText.getPosition().x + scoreStaticText.getGlobalBounds().width + 10,480);

    auto userName = word;
    userName.setString("");
    userName.setOutlineThickness(1);
    userName.setOutlineColor(sf::Color::Green);
    userName.setCharacterSize(23);
    userName.setOrigin(0,0);
    userName.setPosition(nameStaticText.getPosition().x + nameStaticText.getGlobalBounds().width + 10,520);


    auto textWidth = word.getGlobalBounds().width;
    auto textHeight = word.getGlobalBounds().height;
    //SETTING AN ORIGIN TO TEXT (end of word)
    word.setOrigin(textWidth, textHeight / 2);

    word.setPosition(0, windowHeight / 2 - textHeight / 2);


    //CREATING BUTTONS
    auto newGameText = textGenerate("NEW GAME",font,35,sf::Color::White,
                                    sf::Text::Style::Bold);
    auto newGameWidth = newGameText.getGlobalBounds().width;
    auto newGameHeight = newGameText.getGlobalBounds().height;
    newGameText.setOrigin(newGameWidth/2, newGameHeight/2);
    newGameText.setPosition(280,300);

    auto settingsText = textGenerate("SETTINGS",font,35,sf::Color::White,
                                     sf::Text::Style::Bold);
    auto settingsWidth = settingsText.getGlobalBounds().width;
    auto settingsHeight = settingsText.getGlobalBounds().height;
    settingsText.setOrigin(settingsWidth/2, settingsHeight/2);
    settingsText.setPosition(280,400);

    auto infoText = textGenerate("INFO",font,35,sf::Color::White,
                                 sf::Text::Style::Bold);
    auto infoWidth = infoText.getGlobalBounds().width;
    auto infoHeight = infoText.getGlobalBounds().height;
    infoText.setOrigin(infoWidth/2, infoHeight/2);
    infoText.setPosition(280,500);

    auto scoresText = textGenerate("SCORES",font,35,sf::Color::White,
                                   sf::Text::Style::Bold);
    auto scoresWidth = scoresText.getGlobalBounds().width;
    auto scoresHeight = scoresText.getGlobalBounds().height;
    scoresText.setOrigin(scoresWidth/2, scoresHeight/2);
    scoresText.setPosition(280,600);

    auto quitText = textGenerate("QUIT",font,35,sf::Color::White,
                                     sf::Text::Style::Bold);
    auto quitWidth = quitText.getGlobalBounds().width;
    auto quitHeight = quitText.getGlobalBounds().height;
    quitText.setOrigin(quitWidth/2, quitHeight/2);
    quitText.setPosition(280,700);



    auto resumeText = textGenerate("RESUME", font, 35, sf::Color::White,
                                   sf::Text::Style::Bold);
    auto resumeWidth = resumeText.getGlobalBounds().width;
    auto resumeHeight = resumeText.getGlobalBounds().height;
    resumeText.setOrigin(resumeWidth/2, resumeHeight/2);
    resumeText.setPosition(280, 210);

    auto fontSettingsText = textGenerate("FONT",font,35,sf::Color::White,
                                         sf::Text::Style::Bold);
    auto fontSettingsWidth = fontSettingsText.getGlobalBounds().width;
    auto fontSettingsHeight = fontSettingsText.getGlobalBounds().height;
    fontSettingsText.setOrigin(fontSettingsWidth/2,fontSettingsHeight/2);
    fontSettingsText.setPosition(270,340);

    auto diffSettingsText = textGenerate("DIFFICULTY", font,35,sf::Color::White,
                                               sf::Text::Style::Bold);
    auto diffSettingsWidth = diffSettingsText.getGlobalBounds().width;
    auto diffSettingsHeight = diffSettingsText.getGlobalBounds().height;
    diffSettingsText.setOrigin(diffSettingsWidth/2,diffSettingsHeight/2);
    diffSettingsText.setPosition(270,440);



    auto font1Text = textGenerate("FONT",font1,35,sf::Color::White,sf::Text::Style::Bold);
    font1Text.setPosition(500,340);
    auto font2Text = textGenerate("FONT",font2,35,sf::Color::White,sf::Text::Style::Bold);
    font2Text.setPosition(500,400);
    auto font3Text = textGenerate("FONT",font3,35,sf::Color::White,sf::Text::Style::Bold);
    font3Text.setPosition(500,460);
    auto font4Text = textGenerate("FONT(basic)", font,35,sf::Color::White,sf::Text::Style::Bold);
    font4Text.setPosition(500,520);

    auto easyModeText = textGenerate("EASY", font, 35, sf::Color::Green,sf::Text::Style::Bold);
    easyModeText.setPosition(500,340);
    auto mediumModeText = textGenerate("MEDIUM", font, 35, sf::Color::Yellow,sf::Text::Style::Bold);
    mediumModeText.setPosition(500,400);
    auto hardModeText = textGenerate("HARD", font, 35, sf::Color::Red,sf::Text::Style::Bold);
    hardModeText.setPosition(500,460);



    //BLUE STAT BOX
    sf::RectangleShape rectangle(sf::Vector2f(windowWidth, 180));
    rectangle.setFillColor(sf::Color(67,126,245));
    int rectWidth = rectangle.getGlobalBounds().width;
    int rectHeight = rectangle.getGlobalBounds().height;
    rectangle.setPosition(0, windowHeight - rectHeight);
    auto rectPosX = rectangle.getPosition().x;
    auto rectPosY = rectangle.getPosition().y;

    //SETTING POSITION OF STATIC TEXTS ON STAT BOX
    inputText.setPosition(rectPosX+30,rectPosY+30);
    correctText.setPosition(rectPosX+230, rectPosY+30);
    incorrectText.setPosition(rectPosX+430,rectPosY+30);
    missedText.setPosition(rectPosX+630, rectPosY+30);
    timeText.setPosition(rectPosX+30, rectPosY+60);

    //SETTING POSITION OF USER INPUT TEXTS ON STAT BOX
    userText.setPosition(inputText.getPosition().x + inputText.getGlobalBounds().width, windowHeight-180+30);
    userCorrect.setPosition(correctText.getPosition().x+correctText.getGlobalBounds().width, windowHeight-180+30);
    userIncorrect.setPosition(incorrectText.getPosition().x + incorrectText.getGlobalBounds().width, windowHeight-180+30);
    userMissedText.setPosition(missedText.getPosition().x + missedText.getGlobalBounds().width, windowHeight-180+30);
    currentTimeText.setPosition(timeText.getPosition().x + timeText.getGlobalBounds().width, windowHeight-180+60);


    //Clock for words only
    auto clock = sf::Clock();
    //Clock for stars only
    auto clock2 = sf::Clock();
    //Clock for moving line
    auto clock3 = sf::Clock();
    //Clock for general time
    auto generalTime = sf::Clock();
    //CLock for animation
    auto animClock = sf::Clock();

    auto texts = std::vector<sf::Text>();
    auto stars = std::vector<sf::Text>();
    auto movingWords = std::vector<sf::Text>();


    auto yPos = 50;
    auto old_millis = generalTime.getElapsedTime().asMilliseconds();

    int k = 0;


    //DECLARING SPECIFIC BOOLEAN VARIABLES TO CHECK CONDITIONS OF WINDOWS, BUTTONS AND STATES ... ... ... ...
    bool newGamePressed = false;
    bool clearOnlyOnce = false;
    bool isPlaying = false;
    bool paused = false;
    bool isNewGame = true;
    bool resume = false;
    bool restarted = false;
    bool settingsOpen = false;
    bool infoOpen = false;
    bool isGameOver = false;
    bool fontSettingsOpen = false;
    bool diffSettingsOpen = false;
    bool scoresOpen = false;

    bool hatActivated = false;

    bool newGameVisible = true;
    bool settingsVisible = true;
    bool infoVisible = true;
    bool quitVisible = true;
    bool fontSettingsVisible = false;
    bool diffSettingsVisible = false;
    bool easyModeDescrVisible = false;
    bool mediumModeDescrVisible = false;
    bool hardModeDescrVisible = false;
    bool scoresVisible = false;
    bool gameOverInputVisible = true;


    bool font1Visible = false;
    bool font2Visible = false;
    bool font3Visible = false;
    bool font4Visible = false;
    //bool resumeVisible = false;

    bool prevNewGamePressed = newGamePressed;
    bool prevIsPlaying = isPlaying;
    bool prevPaused = paused;
    bool prevIsNewGame = isNewGame;
    bool prevResume = resume;
    bool prevRestarted = restarted;
    bool prevClearOnlyOnce = clearOnlyOnce;
    bool prevSettingsOpen = settingsOpen;
    bool prevInfoOpen = infoOpen;
    bool prevIsGameOver = isGameOver;
    bool prevFontSettingsOpen = fontSettingsOpen;
    bool prevDiffSettingsOpen = diffSettingsOpen;
    bool prevScoresOpen = scoresOpen;


    //USER INPUT TEXT OF STRING TYPE
    auto userInputText = std::string();
    auto userInputName = std::string();
    auto userScoreString = std::string();


    while (window.isOpen()){
        auto event = sf::Event();
        while (window.pollEvent(event)){
            //Check if window is closed
            if(event.type == sf::Event::Closed){
                window.close();
            }
            //Checking for user word input
            if(event.type == sf::Event::TextEntered){

                //checking for any letter
                if(event.text.unicode < 128 and isPlaying){
                    //DELETING ANNOYING \R WHICH WAS THE WHOLE PROBLEM OF WORD MISMATCH. (and \b)
                    if(userInputText.front() == '\r' || userInputText.front() == '\b'){
                        userInputText.erase(userInputText.begin());
                        userText.setString(userInputText);
                    }
                    userInputText += static_cast<char>(event.text.unicode); //converting special code into char by using static_cast
                    userText.setString(userInputText);
                }

                //checking name input while game over SADLY BUT DOES NOT SAVE
                if(event.text.unicode < 128 and isGameOver){
                    userInputName += static_cast<char>(event.text.unicode);
                    if(userInputName.front() == '\r' || userInputName.front() == '\b'){
                        userInputName.erase(userInputName.begin());
                        userName.setString(userInputName);
                    }
                    userName.setString(userInputName);
                }
                //checking backspace while game over
                if(event.text.unicode == 8){
                    if(userInputName.size() > 1){
                        if(userInputName == "\b"){
                            userInputName.clear();
                        }
                        userInputName.pop_back();
                        userInputName.pop_back();
                        userName.setString(userInputName);
                    }
                }

                //checking for backspace
                if(event.text.unicode == 8){
                    if(userInputText.size() > 1){
                        if(userInputText == "\b"){
                            userInputText.clear();
                        }
                        userInputText.pop_back();
                        userInputText.pop_back();
                        userText.setString(userInputText);
                    }
                }

                //checking for ESC key. PAUSING or RESUMING from pause
                if(event.text.unicode == 27){
                    if(!paused){
                        paused = true;
                        resume = false;

                        newGameVisible = true;
                        settingsVisible = true;
                        quitVisible = true;
                        infoVisible = true;
                    }else if(paused and not settingsOpen and not infoOpen and not scoresOpen){ //not settingsOpen because we don't want to resume on Settings close.
                        paused = false;
                        resume = true;

                        newGameVisible = false;
                        settingsVisible = false;
                        quitVisible = false;
                        infoVisible = false;
                    }

                    if(settingsOpen){
                        diffSettingsVisible = false;
                        settingsOpen = false;
                        paused = true;
                        isPlaying = false;

                        newGameVisible = true;
                        settingsVisible = true;
                        quitVisible = true;
                        infoVisible = true;
                    }

                    if(infoOpen){
                        infoOpen = false;
                        paused = true;
                        isPlaying = false;

                        newGameVisible = true;
                        settingsVisible = true;
                        quitVisible = true;
                        infoVisible = true;
                    }

                    if(scoresOpen){
                        scoresOpen = false;
                        paused = true;
                        isPlaying = false;

                        newGameVisible = true;
                        settingsVisible = true;
                        quitVisible = true;
                        infoVisible = true;
                    }


                    if(fontSettingsOpen){
                        fontSettingsOpen = false;
                        settingsOpen = true;
                    }

                    if(diffSettingsOpen){
                        diffSettingsOpen = false;
                        settingsOpen = true;
                    }

                    if(userInputText.size() > 1){
                        //033 - escape character
                        if(userInputText == "\033"){
                            userInputText.clear();
                        }
                        userInputText.pop_back();
                        userInputText.pop_back();
                        userText.setString(userInputText);
                    }
                }

            }

            //Checking for Enter key
            if (event.type == sf::Event::KeyPressed){
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) and isPlaying){
                    //Comparing "Entered" text with the word
                    //std::cout << "Enter key pressed\n";
                    for(int i = 0; i < texts.size(); i++){
                        if (userInputText == texts[i].getString()){

                            userInputText = "";
                            auto emptyString = std::string();
                            userInputText = emptyString;
                            userText.setString(emptyString);
                            texts[i].setString(userInputText);

                            correctCounter++;
                            userCorrect.setString(std::to_string(correctCounter));
                            break;
                        }
                        else if (userInputText != texts[i].getString() and i < texts.size()-1){
                            //DO NOTHING?????? IT WORKS!!!!
                        }
                        else{
                            userInputText = "";
                            auto emptyString = std::string();
                            userInputText = emptyString;
                            userText.setString(emptyString);

                            incorrectCounter++;
                            userIncorrect.setString(std::to_string(incorrectCounter));
                            break;
                        }

                    }
                }

                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) and isGameOver){
                    //userScoreString.clear();
                    userScoreString += userInputName + "     " + userScore.getString();
                    std::cout << userScoreString + "\n";

                    scoreFile << userScoreString;

                    std::ifstream scoresReadFile("../Scores.txt");

                    auto line = std::string();
                    while (std::getline(scoresReadFile, line)){
                        scoresList += line + "\n";
                    }
                    std::cout << scoresList;
                    gameOverInputVisible = false;
                }

                //Checking for specific combinations entered
                //secret hat shortcut
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) and sf::Keyboard::isKeyPressed(sf::Keyboard::H)
                and isPlaying and not hatActivated){
                    hatActivated = true;
                    //font.loadFromFile("../DotsFont.TTF");
                }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) and sf::Keyboard::isKeyPressed(sf::Keyboard::H)
                         and isPlaying and hatActivated){
                    hatActivated = false;
                }
                //quick restart shortcut
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) and sf::Keyboard::isKeyPressed(sf::Keyboard::R)){
                    clearOnlyOnce = false;
                    newGamePressed = true;
                    stars.clear();
                }
                //quick quit shortcut
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) and sf::Keyboard::isKeyPressed(sf::Keyboard::Q)){
                    window.close();
                }
                //quick pause shortcut
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) and sf::Keyboard::isKeyPressed(sf::Keyboard::P)
                and isPlaying){
                    paused = true;
                    resume = false;
                    newGamePressed = false;
                }
                //
//                if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) and sf::Keyboard::isKeyPressed(sf::Keyboard::Equal)
//                and isPlaying){
//                    for(auto text : texts){
//                       text.setCharacterSize(10);
//                       texts.push_back(text);
//                       std::cout << "resized";
//                    }
//                }
            }

            if(event.type == sf::Event::MouseMoved){
                //From website. CHEKING IF OUR MOUSE IS IN BUTTON TEXT'S LOCATION
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f mousePosF( static_cast<float>( mousePos.x ), static_cast<float>( mousePos.y ) );


                if(newGameText.getGlobalBounds().contains(mousePosF))
                    textGenerate(newGameText, sf::Color::Green, sf::Text::Style::Underlined);
                else
                    textGenerate(newGameText, sf::Color::White, sf::Text::Style::Bold);

                if(settingsText.getGlobalBounds().contains(mousePosF))
                    textGenerate(settingsText, sf::Color::Green, sf::Text::Style::Underlined);
                else
                    textGenerate(settingsText, sf::Color::White, sf::Text::Style::Bold);

                if(infoText.getGlobalBounds().contains(mousePosF))
                    textGenerate(infoText, sf::Color::Green, sf::Text::Style::Underlined);
                else
                    textGenerate(infoText, sf::Color::White, sf::Text::Style::Bold);

                if(quitText.getGlobalBounds().contains(mousePosF))
                    textGenerate(quitText, sf::Color::Red, sf::Text::Style::Underlined);
                else
                    textGenerate(quitText, sf::Color::White, sf::Text::Style::Bold);

                if(resumeText.getGlobalBounds().contains(mousePosF))
                    textGenerate(resumeText, sf::Color::Green, sf::Text::Style::Underlined);
                else
                    textGenerate(resumeText, sf::Color::White, sf::Text::Style::Bold);

                if(fontSettingsText.getGlobalBounds().contains(mousePosF))
                    textGenerate(fontSettingsText, sf::Color::Green, sf::Text::Style::Underlined);
                else
                    textGenerate(fontSettingsText, sf::Color::White, sf::Text::Style::Bold);

                if(font1Text.getGlobalBounds().contains(mousePosF) and font1Visible)
                    textGenerate(font1Text, sf::Color::Green, sf::Text::Style::Underlined);
                else
                    textGenerate(font1Text, sf::Color::White, sf::Text::Style::Bold);

                if(font2Text.getGlobalBounds().contains(mousePosF) and font2Visible)
                    textGenerate(font2Text, sf::Color::Green, sf::Text::Style::Underlined);
                else
                    textGenerate(font2Text, sf::Color::White, sf::Text::Style::Bold);

                if(font3Text.getGlobalBounds().contains(mousePosF) and font3Visible)
                    textGenerate(font3Text, sf::Color::Green, sf::Text::Style::Underlined);
                else
                    textGenerate(font3Text, sf::Color::White, sf::Text::Style::Bold);

                if(font4Text.getGlobalBounds().contains(mousePosF) and font4Visible)
                    textGenerate(font4Text,sf::Color::Green,sf::Text::Style::Underlined);
                else
                    textGenerate(font4Text,sf::Color::White,sf::Text::Style::Bold);

                if(diffSettingsText.getGlobalBounds().contains(mousePosF))
                    textGenerate(diffSettingsText,sf::Color::Green, sf::Text::Style::Underlined);
                else
                    textGenerate(diffSettingsText,sf::Color::White,sf::Text::Style::Bold);

                if(easyModeText.getGlobalBounds().contains(mousePosF)){
                    textGenerate(easyModeText,sf::Color::Green, sf::Text::Style::Underlined, 1);
                    easyModeDescrVisible = true;
                }
                else{
                    textGenerate(easyModeText,sf::Color::Green, sf::Text::Style::Bold, 0);
                    easyModeDescrVisible = false;
                }

                if(mediumModeText.getGlobalBounds().contains(mousePosF)){
                    textGenerate(mediumModeText,sf::Color::Yellow, sf::Text::Style::Underlined, 1);
                    mediumModeDescrVisible = true;
                }
                else{
                    textGenerate(mediumModeText,sf::Color::Yellow, sf::Text::Style::Bold, 0);
                    mediumModeDescrVisible = false;
                }

                if(hardModeText.getGlobalBounds().contains(mousePosF)){
                    textGenerate(hardModeText,sf::Color::Red, sf::Text::Style::Underlined, 1);
                    hardModeDescrVisible = true;
                }
                else{
                    textGenerate(hardModeText,sf::Color::Red, sf::Text::Style::Bold, 0);
                    hardModeDescrVisible = false;
                }

                if(scoresText.getGlobalBounds().contains(mousePosF)){
                    textGenerate(scoresText,sf::Color::Green, sf::Text::Style::Underlined, 0);
                    scoresVisible = true;
                }
                else{
                    textGenerate(scoresText,sf::Color::White, sf::Text::Style::Bold, 0);
                    scoresVisible = false;
                }


            }


            if(event.type == sf::Event::MouseButtonPressed and !isPlaying or event.type == sf::Event::MouseButtonReleased and !isPlaying){
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f mousePosF( static_cast<float>( mousePos.x ), static_cast<float>( mousePos.y ) );

                //Checking if NEW GAME button was pressed
                if(newGameText.getGlobalBounds().contains(mousePosF) and newGameVisible){
                    isNewGame = false;
                    newGamePressed = true;

                    paused = false;

                    isPlaying = true;
                    isGameOver = false; //???????
                    break;
                }
                //Checking if RESUME button was pressed
                if(resumeText.getGlobalBounds().contains(mousePosF)){
                    paused = false;
                    resume = true;

                    isPlaying = true;
                    break;
                }
                //Checking if SETTINGS button was pressed
                if(settingsText.getGlobalBounds().contains(mousePosF) and settingsVisible){
                    settingsOpen = true;
                    fontSettingsOpen = false;
                    diffSettingsOpen = false;
                    //paused = true;
                    //resume = false;
                    isPlaying = false;
                    break;
                }
                //Checking if INFO button was pressed
                if(infoText.getGlobalBounds().contains(mousePosF) and infoVisible){
                    infoOpen = true;
                    isPlaying = false;
                    break;
                }
                if(scoresText.getGlobalBounds().contains(mousePosF) and scoresVisible){
                    scoresOpen = true;
                    isPlaying = false;
                    break;
                }
                //Checking if QUIT button was pressed
                if(quitText.getGlobalBounds().contains(mousePosF) and quitVisible){
                    window.close();
                }
                //Checking for FONT button
                if(fontSettingsText.getGlobalBounds().contains(mousePosF)){
                    fontSettingsOpen = true;
                    diffSettingsOpen = false;
                    break;
                }
                //Checking for difficulty button
                if(diffSettingsText.getGlobalBounds().contains(mousePosF) and diffSettingsVisible){
                    diffSettingsOpen = true;
                    fontSettingsOpen = false;
                    break;
                }
                //Checking for font selection buttons
                if(font1Text.getGlobalBounds().contains(mousePosF) and font1Visible){
                    font = font1;
                }
                if(font2Text.getGlobalBounds().contains(mousePosF)  and font2Visible){
                    font = font2;
                }
                if(font3Text.getGlobalBounds().contains(mousePosF)  and font3Visible){
                    font = font3;
                }
                if(font4Text.getGlobalBounds().contains(mousePosF)  and font4Visible){
                    font = font4;
                }
                //Checking for difficulty pressed
                if(easyModeText.getGlobalBounds().contains(mousePosF) and isNewGame
                or easyModeText.getGlobalBounds().contains(mousePosF) and isGameOver){
                    spawnRate = 3;
                    speedOfWords = 0.9;
                    minWordRand = 1;
                    maxWordRand = 2;
                    minWordSize = 20;
                    maxWordSize = 20;
                    prevSpeedOfWords = speedOfWords;
                    prevMinWordRand = minWordRand;
                    prevMaxWordRand = maxWordRand;
                }
                if(mediumModeText.getGlobalBounds().contains(mousePosF) and isNewGame
                or mediumModeText.getGlobalBounds().contains(mousePosF) and isGameOver){
                    spawnRate = 2;
                    speedOfWords = 1.5;
                    minWordRand = 1;
                    maxWordRand = 3;
                    minWordSize = 17;
                    maxWordSize = 21;
                    prevSpeedOfWords = speedOfWords;
                    prevMinWordRand = minWordRand;
                    prevMaxWordRand = maxWordRand;
                }
                if(hardModeText.getGlobalBounds().contains(mousePosF) and isNewGame
                or hardModeText.getGlobalBounds().contains(mousePosF) and isGameOver){
                    spawnRate = 2;
                    speedOfWords = 2;
                    minWordRand = 1;
                    maxWordRand = 4;
                    minWordSize = 14;
                    maxWordSize = 27;
                    prevSpeedOfWords = speedOfWords;
                    prevMinWordRand = minWordRand;
                    prevMaxWordRand = maxWordRand;
                }


            }
        }

        //check word out of the scope
        if(word.getPosition().x == windowWidth){
            if(isPlaying){
                incorrectCounter++;
                missedCounter++;
                userIncorrect.setString(std::to_string(incorrectCounter));
                userMissedText.setString(std::to_string(missedCounter));
            }
        }

        //SPAWNING RANDOM AMOUNT OF WORDS EVERY N SECONDS.
        if(!paused){
            auto randAmount = randomNumGenerator(minWordRand, maxWordRand);
            if(clock.getElapsedTime().asSeconds() >= spawnRate){
                minYRand = 10;
                //dividing allowed Y size of space to spawn words by random amount of them.
                maxYRand = 620/randAmount;
                for(int i = 0; i < randAmount; ++i){
                    sf::Text newText = textGenerate(words[k],font,
                                                    randomNumGenerator(minWordSize,maxWordSize),
                                                    sf::Color::Green,sf::Text::Style::Regular);
                    newText.setPosition(randomNumGenerator(minXRand, maxXRand),
                                        randomNumGenerator(minYRand,maxYRand));
                    texts.push_back(newText);
                    //MAKING INFINITE LOOP OF THE SAME WORDS
                    if(k < words.size() - 1 ){
                        k++;
                    }
                    else{
                        k = 0;
                    }
                    minYRand+=620/randAmount;
                    maxYRand+=620/randAmount;
                }
                clock.restart();
            }
        }


        //Spawning stars:
        auto starsPerSecond = 10;
        //int k = 0;
        if(clock2.getElapsedTime().asSeconds() > 1){
            auto minStarSpawnY = 10;
            auto maxStarSpawnY = 640 / starsPerSecond;
            auto minStarSpawnX = -200;
            auto maxStarSpawnX = -10;

            for(int i = 0; i < starsPerSecond; i++){
                sf::Text newStar = textGenerate(".",font,20,
                                                sf::Color(255,255,255,75),sf::Text::Style::Regular);
                newStar.setPosition(randomNumGenerator(minStarSpawnX,maxStarSpawnX),
                                    randomNumGenerator(minStarSpawnY, maxStarSpawnY));
                //making outline to make stars look "Glowy"
                newStar.setOutlineThickness(1.5);
                newStar.setOutlineColor(sf::Color(120,120,120));
                stars.push_back(newStar);

                minStarSpawnY+= 640 / starsPerSecond;
                maxStarSpawnY+= 640 / starsPerSecond;
            }
                clock2.restart();
        }

        //Creating a moving line words
        if(clock3.getElapsedTime().asSeconds() > 2.6){
            auto newMovingWord = textGenerate("MONKEY TYPER",vcrFont,60,
                                              sf::Color::White,sf::Text::Style::Italic);
            newMovingWord.setPosition(-650,40);
            newMovingWord.setOutlineThickness(2);
            newMovingWord.setOutlineColor(sf::Color(67,126,245));
            movingWords.push_back(newMovingWord);
            clock3.restart();
        }


        //Moving words and checking for specific conditions
        for(auto& text : texts){
            text.move(speedOfWords, 0);

            if (text.getPosition().x + text.getGlobalBounds().width> window50p){
                text.setFillColor(sf::Color(245, 238, 42));


            }
            if (text.getPosition().x  + text.getGlobalBounds().width> window80p){
                text.setFillColor(sf::Color(242, 85, 53));
            }

            //CLEARING ELEMENTS FROM THE VECTOR WHICH HAVE PASSED OUT OF THE WINDOW'S SCOPE.
            if(text.getPosition().x >= windowWidth){
                // checking each iterator from the beginning till the end
                for(auto it = texts.begin(); it != texts.end();){
                    //deleting the specific element under a condition.
                    //ALSO INCREMENTING INCORRECT COUNTER AS THE WORD IS OUT OF SCOPE OF VIEW.
                    //has to be fixed!
                    if(it->getPosition().x >= windowWidth){

                        //Not empty string = not entered word = ++incorrectCounter
                        if(it->getString() != ""){
                            incorrectCounter++;
                            missedCounter++;
                            userIncorrect.setString(std::to_string(incorrectCounter));
                            userMissedText.setString(std::to_string(missedCounter));

                            //Checking if bigger than 10. If so - game over.
                            if(missedCounter>=10 and isPlaying){
                                isGameOver = true;
                            }
                        }
                        texts.erase(it);
                        //std::cout << "Size of the vector: " << texts.size() << "\n";
                    }
                    //incrementing and moving forward if such element was not found
                    else{
                        ++it;
                    }
                }
            }
        }

        //Moving stars and checking specific conditions:
        for(auto& star : stars){
            star.move(speedOfStars,0);
            //a loop which goes through the whole vector by iterators
            for(auto it = stars.begin(); it != stars.end();){
                if(it->getPosition().x >= windowWidth){
                    stars.erase(it);
                    //std::cout << "Size of star vector: " << stars.size() << "\n";
                }
                //if not found - moving forward
                else{
                    ++it;
                }
            }

        }

        //Moving "Moving words" and checking specific conditions
        for(auto& movingWord : movingWords){
            movingWord.move(speedOfMovingWords,0);
            //a loop which goes through the whole vector by iterators
            for(auto it = movingWords.begin(); it != movingWords.end();){
                if(it->getPosition().x >= windowWidth){
                    movingWords.erase(it);
                    //std::cout << "Size of moving word vector: " << movingWords.size() << "\n";
                }
                    //if not found - moving forward
                    else{
                    ++it;
                }
            }
        }

        //setting general time to clock
        if(generalTime.getElapsedTime().asMilliseconds() - old_millis >= 1000) {

            auto millis = generalTime.getElapsedTime().asMilliseconds();
            auto minutes = (int) millis / 60000;
            auto seconds = (int) (millis - minutes * 60000) / 1000;


            auto minutes_str = std::string();
            auto seconds_str = std::string();

            if (minutes == 0) {
                minutes_str = "00";
            } else if (minutes < 10) {
                minutes_str = "0" + std::to_string(minutes);
            } else {
                minutes_str = std::to_string(minutes);
            }

            if (seconds == 0) {
                seconds_str = "00";
            } else if (seconds < 10) {
                seconds_str = "0" + std::to_string(seconds);
            } else {
                seconds_str = std::to_string(seconds);
            }
            currentTimeText.setString(minutes_str + " : " + seconds_str);
            old_millis = millis;
        }


        window.clear(sf::Color::Black);


        //visuals: (sprites)
        window.draw(leftSideGradientSprite);
        window.draw(rightSideGradientSprite);



        //DRAWING WINDOWS UNDER SPECIFIC CONDITIONS
        //First main menu enter onStart
        if(isNewGame){

            diffSettingsVisible = false;
            newGamePressed = false;
            paused = false;
            isPlaying = false;
            //drawing stars as a background
            for(const auto& star : stars){
                window.draw(star);
            }
            //drawing words as a background
            for(const auto& text : texts){
                window.draw(text);
            }
            //baseDrawMainMenu(window,mainMenuSprite,movingLineGradientSprite,movingWords,newGameText,settingsText,infoText,quitText);

            window.draw(mainMenuSprite);
            window.draw(movingLineGradientSprite);
            //drawing running stripe LOGO words
            for(const auto& mowingWord : movingWords){
                window.draw(mowingWord);
            }
            //Drawing main menu buttons
            window.draw(newGameText);
            window.draw(settingsText);
            window.draw(infoText);
            window.draw(quitText);
            window.draw(scoresText);


        }


        if(newGamePressed){
            newGameVisible = false;
            settingsVisible = false;
            quitVisible = false;
            infoVisible = false;
            scoresVisible = false;


            //Checking if newGame button was pressed during a pause. If so - we restart (basically a new game)
            if(prevPaused){
                clearOnlyOnce = false;
                restarted = true;
            }
            speedOfWords = prevSpeedOfWords;
            speedOfStars = prevsSpeedOfStars;


            isPlaying = true;
            //paused = false;
            isNewGame = false;
            //resume = false;

            if(!clearOnlyOnce){
                correctCounter = 0;
                userCorrect.setString(std::to_string(correctCounter));
                //userScore.setString(std::to_string(correctCounter));
                incorrectCounter = 0;
                userIncorrect.setString(std::to_string(incorrectCounter));
                missedCounter = 0;
                userMissedText.setString(std::to_string(missedCounter));
                //clearing vector of sf::Text-s
                texts.clear();
                //clearing vector of Stars if restarted
                if(restarted){
                    stars.clear();
                }
                //clearing users' input field
                userInputText.clear();

                userText.setString(userInputText);
                clearOnlyOnce = true;
            }

            //DRAWING ELEMENTS FOR GAMEPLAY:
            //window.draw(rectangle);
            window.draw(statsBoxSprite);
            window.draw(userText);
            window.draw(userCorrect);
            window.draw(userIncorrect);
            window.draw(userMissedText);
            window.draw(timeText);
            //texts unchangable
            window.draw(correctText);
            window.draw(incorrectText);
            window.draw(inputText);
            window.draw(missedText);
            window.draw(currentTimeText);


            //drawing stars
            for(const auto& star : stars){
                window.draw(star);
            }
            //drawing words
            for(const auto& text : texts){
                window.draw(text);
            }

            window.draw(leftSideGradientSprite);
            window.draw(rightSideGradientSprite);

            window.draw(basicGuySprite);
            if(missedCounter<5){
                window.draw(thumbsUpSprite);
            }
            else if(missedCounter>=8){
                window.draw(thumbsDownSprite);
            }

            if(hatActivated){
                window.draw(hatSprite);
            }


        }
        if(paused){
            newGameVisible = true;
            settingsVisible = true;
            quitVisible = true;
            infoVisible = true;
            scoresVisible = true;

            speedOfWords = 0;
            speedOfStars = 0;
            starsPerSecond = 0;



            newGamePressed = false;
            paused = true;
            isPlaying = false;
            resume = false;


            //drawing stars as a background
            for(const auto& star : stars){
                window.draw(star);
            }
            //drawing words as a background
            for(const auto& text : texts){
                window.draw(text);
            }

            //drawing visuals
            window.draw(mainMenuSprite);
            window.draw(movingLineGradientSprite);
            //drawing running stripe LOGO words
            for(const auto& mowingWord : movingWords){
                window.draw(mowingWord);
            }
            //Drawing main menu buttons
            window.draw(resumeText);
            window.draw(newGameText);
            window.draw(settingsText);
            window.draw(infoText);
            window.draw(quitText);
            window.draw(scoresText);
        }

        if(resume and not paused){
            newGameVisible = false;
            settingsVisible = false;
            quitVisible = false;
            infoVisible = false;
            scoresVisible = false;

            speedOfWords = prevSpeedOfWords;
            speedOfStars = prevsSpeedOfStars;

            newGamePressed = false;
            paused = false;
            isPlaying = true;

            //resume = true;


            //DRAWING ELEMENTS FOR GAMEPLAY
            //window.draw(rectangle);
            window.draw(statsBoxSprite);
            window.draw(userText);
            window.draw(userCorrect);
            window.draw(userIncorrect);
            window.draw(userMissedText);
            window.draw(timeText);
            //texts unchangable
            window.draw(correctText);
            window.draw(incorrectText);
            window.draw(inputText);
            window.draw(missedText);
            window.draw(currentTimeText);

            //drawing stars
            for(const auto& star : stars){
                window.draw(star);
            }
            //drawing words
            for(const auto& text : texts){
                window.draw(text);
            }

            window.draw(leftSideGradientSprite);
            window.draw(rightSideGradientSprite);

            window.draw(basicGuySprite);
            if(missedCounter<5){
                window.draw(thumbsUpSprite);
            }
            else if(missedCounter>=8){
                window.draw(thumbsDownSprite);
            }

            if(hatActivated){
                window.draw(hatSprite);
            }

        }
        if(isGameOver){
            userScore.setString(std::to_string(correctCounter));
            newGameVisible = true;
            settingsVisible = true;
            quitVisible = true;
            infoVisible = true;
            scoresVisible = true;

            speedOfStars= 0;
            speedOfWords = 0;

            starsPerSecond = 0;

            newGamePressed = false;
            paused = false;
            isPlaying = false;
            clearOnlyOnce = false;
            //drawing stars as a background
            for(const auto& star : stars){
                window.draw(star);
            }
            //drawing words as a background
            for(const auto& text : texts){
                window.draw(text);
            }
            //baseDrawMainMenu(window,mainMenuSprite,movingLineGradientSprite,movingWords,newGameText,settingsText,infoText,quitText);
            window.draw(mainMenuSprite);
            window.draw(bigMenuSprite);
            window.draw(movingLineGradientSprite);
            //drawing running stripe LOGO words
            for(const auto& mowingWord : movingWords){
                window.draw(mowingWord);
            }
            //Drawing main menu buttons
            window.draw(newGameText);
            window.draw(settingsText);
            window.draw(infoText);
            window.draw(quitText);
            window.draw(scoresText);

            window.draw(gameOverDescrText);
            window.draw(separatorSprite);
            window.draw(scoreStaticText);
            window.draw(userScore);
            if (gameOverInputVisible){
                window.draw(nameStaticText);
                window.draw(userName);
            }

        }
        if(settingsOpen or settingsOpen and isGameOver){
            //isGameOver = false;
            newGameVisible = false;
            settingsVisible = false;
            quitVisible = false;
            infoVisible = false;
            diffSettingsVisible = true;

            if(not isNewGame){
                speedOfWords = 0;
                speedOfStars = 0;
            }


            newGamePressed = false;
            paused = true;
            isPlaying = false;
            resume = false;
            isPlaying = false;

            //drawing stars as a background
            for(const auto& star : stars){
                window.draw(star);
            }
            //drawing words as a background
            for(const auto& text : texts){
                window.draw(text);
            }
            //drawing visuals

            window.draw(mainMenuSprite);
            window.draw(bigMenuSprite);
            window.draw(movingLineGradientSprite);
            window.draw(fontSettingsText);
            window.draw(diffSettingsText);
            window.draw(separatorSprite);

            if(fontSettingsOpen){
                diffSettingsOpen = false;
                window.draw(font1Text);
                window.draw(font2Text);
                window.draw(font3Text);
                window.draw(font4Text);

                font1Visible = true;
                font2Visible = true;
                font3Visible = true;
                font4Visible = true;
            }else{
                font1Visible = false;
                font2Visible = false;
                font3Visible = false;
                font4Visible = false;
            }


            if(diffSettingsOpen){
                fontSettingsOpen = false;
                window.draw(easyModeText);
                window.draw(mediumModeText);
                window.draw(hardModeText);
                if(easyModeDescrVisible){
                    window.draw(easyModeDescrText);
                }
                if(mediumModeDescrVisible){
                    window.draw(mediumModeDescrText);
                }
                if(hardModeDescrVisible){
                    window.draw(hardModeDescrText);
                }
            }
            //drawing running stripe LOGO words
            for(const auto& mowingWord : movingWords){
                window.draw(mowingWord);
            }
        }

        if(infoOpen){
            if(not isNewGame){
                speedOfWords = 0;
                speedOfStars = 0;
            }

            newGamePressed = false;
            paused = true;
            isPlaying = false;
            resume = false;
            isPlaying = false;

            //drawing stars as a background
            for(const auto& star : stars){
                window.draw(star);
            }
            //drawing words as a background
            for(const auto& text : texts){
                window.draw(text);
            }
            //drawing visuals

            window.draw(mainMenuSprite);
            window.draw(bigMenuSprite);
            window.draw(movingLineGradientSprite);
            window.draw(infoDescrText);
            window.draw(separatorSprite);

            //drawing running stripe LOGO words
            for(const auto& mowingWord : movingWords){
                window.draw(mowingWord);
            }
        }

        if(scoresOpen){
            if(not isNewGame){
                speedOfWords = 0;
                speedOfStars = 0;
            }

            newGamePressed = false;
            paused = true;
            isPlaying = false;
            resume = false;
            isPlaying = false;

            //drawing stars as a background
            for(const auto& star : stars){
                window.draw(star);
            }
            //drawing words as a background
            for(const auto& text : texts){
                window.draw(text);
            }
            //drawing visuals

            window.draw(mainMenuSprite);
            window.draw(bigMenuSprite);
            window.draw(movingLineGradientSprite);

            window.draw(separatorSprite);
            window.draw(scoresListText);

            //drawing running stripe LOGO words
            for(const auto& mowingWord : movingWords){
                window.draw(mowingWord);
            }
        }


        //SHOWING ALL BOOL VARIABLES NOT TO MIX UP
        if(newGamePressed != prevNewGamePressed or
        isPlaying != prevIsPlaying or
        paused != prevPaused or
        isNewGame != prevIsNewGame or
        resume != prevResume or
        restarted != prevRestarted or
        clearOnlyOnce != prevClearOnlyOnce or
        settingsOpen != prevSettingsOpen or
        infoOpen != prevInfoOpen or
        isGameOver != prevIsGameOver){
            std::cout << "\n\nNewGamePressed " << newGamePressed
            << "\nisPlaying " << isPlaying
            << "\nPaused " << paused
            << "\nisNewGame " << isNewGame
            << "\nResume " << resume
            << "\nRestarted " << restarted
            << "\nClearOnlyOnce " << clearOnlyOnce
            << "\nSettingsOpen " << settingsOpen
            << "\ninfoOpen " << infoOpen
            << "\nFontSettingsOpen " << fontSettingsOpen
            << "\nIsGameOVer: " << isGameOver;

            prevNewGamePressed = newGamePressed;
            prevIsPlaying = isPlaying;
            prevPaused = paused;
            prevIsNewGame = isNewGame;
            prevResume = resume;
            prevRestarted = restarted;
            prevClearOnlyOnce = clearOnlyOnce;
            prevSettingsOpen = settingsOpen;
            prevInfoOpen = infoOpen;
            prevFontSettingsOpen = fontSettingsOpen;
            prevIsGameOver = isGameOver;
        }

        window.display();
    }
    return 0;
}