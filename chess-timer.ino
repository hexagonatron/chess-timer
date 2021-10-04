const int btn1Pin = 2;
const int btn2Pin = 3;

const long millisInSec = 1000UL;
const long millisInMin = millisInSec * 60;
const long millisInHour = millisInMin * 60;

const long debounceTime = 50UL;

const int btnNum = 2;
int btnPins[btnNum] = {btn1Pin, btn2Pin};
bool currBtnState[btnNum] = {false, false};
bool lastBtnState[btnNum] = {false, false};
long lastBtnTime[btnNum] = {0UL, 0UL};

void setup() {
    Serial.begin(115200);
    pinMode(btn1Pin, INPUT_PULLUP);
    pinMode(btn2Pin, INPUT_PULLUP);
}

long playerOneTime = 1000UL*62;
long playerTwoTime = 1000UL*62;

long playerMillis[] =  {playerOneTime, playerTwoTime};

long startTime = 0UL;
long lastMillis = 0UL;

int activePlayer = 0;

bool isRunning = false;

void loop() {
    start();
    updateTime();
    checkBtns();
    display();
    delay(50);
}

void checkBtns() {
    debounceBtns();
    updateActivePlayer();
}

void updateActivePlayer() {
    if (activePlayer == 0) {
        if (isNewBtnPushDown(0)) {
            activePlayer = 1;
        }
    } else if (activePlayer == 1) {
        if (isNewBtnPushDown(1)) {
            activePlayer = 0;
        }
    }
}

bool isNewBtnPushDown(int btnIndex) {
    return currBtnState[btnIndex] && !lastBtnState[btnIndex];
}

void debounceBtns() {
    for (int i = 0; i < btnNum; i++) {
        debounceBtn(i);
    }
}

void debounceBtn(int btnIndex) {
        bool currState = digitalRead(btnPins[btnIndex]);
        long currTime = millis();
        if (currTime - lastBtnTime[btnIndex] > debounceTime) {
            lastBtnState[btnIndex] = currBtnState[btnIndex];
            currBtnState[btnIndex] = currState;
            lastBtnTime[btnIndex] = currTime;
        }
}

void start() {
    if (!isRunning) {
        lastMillis = millis();
        isRunning = true;
    }
}

long lastDisplay = 0UL;
long displayInterval = 50;

bool shouldUpdate() {
    long currentMillis = millis();
    long difference = currentMillis - lastDisplay;
    if (difference >= displayInterval) {
        return true;
    }
    return false;
}

void display () {
    if (!shouldUpdate) {
        Serial.print("no");
        return;
    }
    lastDisplay = millis();
    displaySerial(playerOneTime, playerTwoTime);
}

void displaySerial(long playerOneTime, long playerTwoTime) {
    Serial.print("Player 1: ");
    Serial.print(formatTime(playerMillis[0]));
    Serial.print(" Player 2: ");
    Serial.println(formatTime(playerMillis[1]));
}

String formatTime(long timeInMillis) {
    long hours = timeInMillis / millisInHour;
    long minutes = ( timeInMillis % millisInHour ) / millisInMin;
    long seconds = ( timeInMillis % millisInMin ) / millisInSec;
    long millis = timeInMillis % millisInSec;
    long tenths = millis / 100;

    if (minutes < 1) {
        String secStr = padLeft(seconds);
        String tenthStr= String(tenths);
        return secStr + "." + tenthStr;
    }
    if (hours < 1) {
        String minStr = padLeft(minutes);
        String secStr = padLeft(seconds);
        return minStr + ":" + secStr;
    }
    return String(hours) + ":" + padLeft(minutes) + ":" + padLeft(seconds);

}

String padLeft(long input) {
    if (input < 10) {
        return "0" + String(input);
    }
    return String(input);
}

void updateTime() {
    long currentMillis = millis();
    long difference = currentMillis - lastMillis;
    playerMillis[activePlayer] -= difference;
    if (playerMillis[activePlayer] < 0) {
        playerMillis[activePlayer] = 0;
    }
    lastMillis = currentMillis;
}