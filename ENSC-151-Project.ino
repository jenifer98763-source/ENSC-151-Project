// ENSC 151 Project - Main file
/** Description:
 * - Introductory display code
 * - Determines difficulty
 * - Randomizes puzzles and puzzle order
 * - Runs puzzles
 * - Opens the box (servo)
 */
#include "puzzles.h"

rgb_lcd lcd;
Servo groveServo;

// GAME STATE
Puzzle* allPuzzles[] = { &Touch_Count, &Light_Lock_Puzzle, &Dial_Puzzle,
                         &Temperature_Puzzle, &Clapping_Count, &Introduction_Diplay,
                         &Servo_Code, &Secret_Press };
const int NUM_PUZZLES = 6; // Change depending on how many puzzles are added CHANGE THIS

Puzzle* order[NUM_PUZZLES];   // shuffled puzzles for this game
int puzzleCount = 2;          // how many to solve (set by difficulty) CHANGE THIS
int current = 0;              // index of the active puzzle
bool gameOver = false;
int difficulty = 0; // 0 = Easy, 1 = Medium, 2 = Hard
String names[] = {"Easy", "Medium", "Hard"};

void beepSuccess() {
  tone(PIN_BUZZER, 1000, 200);
}

void showError(const char* message) {
  lcd.clear();
  lcd.print("placeholder");
}

// void setDifficultyColor() {
//   if (difficulty == 0) {
//     lcd.setRGB(0, 255, 0);
//   } else if (difficulty == 1) {
//       lcd.setRGB(255, 90, 0);
//     } else {
//       lcd.setRGB(255, 0, 0);
//     }
// }

// void showDifficulty() {
//   setDifficultyColor();
//   lcd.clear();
//   lcd.setCursor(0, 0);
//   lcd.print("Mode: " + names[difficulty]);
//   lcd.setCursor(0, 1);
//   lcd.print("Tap:next Hold:OK");
// }

// void chooseDifficulty() {
//   showDifficulty();
//   while (true) {
//     if (digitalRead(PIN_BUTTON) == HIGH) {
//       unsigned long start = millis();
//       while (digitalRead(PIN_BUTTON) == HIGH) {}   // wait for release

//       if (millis() - start >= 1000) {
//         return;                                    // held: confirm
//       }
//       difficulty = (difficulty + 1) % 3;           // tapped: next option
//       showDifficulty();
//       delay(50);                                   
//     }
//   }

//   // Print selected difficulty
//   lcd.clear();
//   lcd.print("Selected:");
//   lcd.setCursor(0, 1);
//   lcd.print(names[difficulty]);
//   delay(1500);
//   lcd.clear();

//   // Set puzzle count
//   if (difficulty == 0) {
//     // Easy: 2
//     puzzleCount = 2;
//   } else if (difficulty == 1) {
//     // Medium: 4
//     puzzleCount = 4;
//   } else {
//     // Hard: 6
//     puzzleCount = 6;
//   }
// }

// Shuffle puzzles randomly
void shufflePuzzles() {
  for (int i = 0; i < NUM_PUZZLES; i++) {
    // Put puzzles into order array
    order[i] = allPuzzles[i];
  }
  for (int i = NUM_PUZZLES - 1; i > 0; i--) {
    // Randomize swapping puzzle with a range of how many puzzles left
    int j = random(i + 1);
    // Create temp puzzle value to swap
    Puzzle* temp = order[i];
    order[i] = order[j];
    order[j] = temp;
  }
}

// Game start logic
void startGame() {

  // Shuffle all puzzles
  shufflePuzzles();
  current = 0;
  gameOver = false;

  // Start first puzzle
  order[current]->begin();
}

void setup() {
  // Welcome message
  lcd.begin(16, 2);
  lcd.setRGB(0, 150, 255);

  lcd.setCursor(3, 0);
  lcd.print("Welcome to");
  lcd.setCursor(2, 1);
  lcd.print("Mini Escape!");
  delay(3000);

  groveServo.attach(PIN_SERVO);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_BUTTON, INPUT);
  randomSeed(analogRead(PIN_SOUND) + micros()); // random seed once using a mixed signal

  // User chooses difficulty
  // chooseDifficulty();
  startGame();
}

void loop() {
  if (gameOver) return; // box is open, fully completed

  if (order[current]->update()) { // Start "loop()" for the active puzzle
    beepSuccess();
    // Move index to next puzzle
    current++;

    // Check if all puzzles in difficulty are completed
    if (current >= puzzleCount) {
      lcd.clear();
      lcd.print("Box unlocked!");
      gameOver = true;
    } else {
      order[current]->begin(); // start the next puzzle
    }
  }
}
