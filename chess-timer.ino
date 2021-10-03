const long millisInSec = 1000UL;
const long millisInMin = millisInSec * 60;
const long millisInHour = millisInMin * 60;

void setup() {
    Serial.begin(115200);

}

long playerOneTime = 1000UL*62;
long playerTwoTime = 1000UL*62;

long playerMillis[] =  {playerOneTime, playerTwoTime};

long startTime = 0UL;
long lastMillis = 0UL;

int activePlayer = 1;

bool isRunning = false;

void loop() {
    start();
    display();
    updateTime();
    delay(50);

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