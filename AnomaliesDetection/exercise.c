#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SPEEDOMETER_FREQUENCY 50
#define PEDALS_FREQUENCY 5
#define ABS_FREQUENCY 10
#define TIRE_PRESSURE_FREQUENCY 100
#define SPEEDOMETER_ID 0x100
#define PEDALS_ID 0x200
#define Abs_ID 0x400
#define TIRE_PRESSURE_ID 0x800
#define TIMING_ANOMALIES 1
#define BEHAVIORAL_ANOMALIES 2
#define CORRELATION_ANOMALIES 3
#define NO_ANOMALY_DETECTED 0
#define ANOMALY_DETECTED 1
#define MAX_ANOMALIES 1000
#define FIXED_VALUE 0
#define MAX_SPEEDOMETER_VAL 300
#define MAX_PEDALS_VAL 100
#define MAX_ABS_VAL 1
#define MAX_TIRE_PRESSURE_VAL 100
#define TRUE 1
#define FALSE 0
#define HARD_PRESS 80

typedef unsigned int uint;

typedef struct speedometer {
	uint lastTransmit;
	uint val1;
	uint val2;
	uint shouldAccelerate;
	uint shouldSlowDown;
}Speedometer;
typedef struct pedals {
	uint lastTransmit;
	uint val1;
	uint val2;
	uint brakePressTime;
	uint gasPressTime;
}Pedals;
typedef struct abs {
	uint lastTransmit;
	uint val1;
	uint val2;
}Abs;
typedef struct tirePressure {
	uint lastTransmit;
	uint val1;
	uint val2;
}TirePressure;
typedef struct message {
	uint messageId;
	uint timestamp;
	uint ecuId;
	uint val1;
	uint val2;
}Message;
typedef struct car {
	uint isActive;
	Speedometer speedometer;
	Pedals pedals;
	Abs abs;
	TirePressure tirePressure;
}Car;

/*main functions prototypes*/
int detect_timing_anomalies(const char* file_path, unsigned int *anomalies_ids);
int detect_behavioral_anomalies(const char* file_path, unsigned int *anomalies_ids);
int detect_correlation_anomalies(const char* file_path, unsigned int *anomalies_ids);
void detectAnomalies(FILE *file, unsigned int *anomalies_ids, uint anomalyType);
int checkForTimingAnomaly(Message *currentMessage, Car *car);
int checkForBehavioralAnomaly(Message *currentMessage, Car *car);
int checkForCorrelationAnomaly(Message *currentMessage, Car *car);

/*initialization prototypes*/
void intialComponents(Car *car);
void intialTirePressure(TirePressure *tirePressure);
void intialSpeedometer(Speedometer *speedometer);
void intialPedals(Pedals *pedals);
void intialAbs(Abs *abs);

/*updates & validation prototypes*/
void getMessage(char *line, Message *currentMessage);
void updateSpeedometerState(Message *currentMessage, Speedometer *speedometerState);
void updatePedalsState(Message *currentMessage, Pedals *pedalsState);
void updateAbsState(Message *currentMessage, Abs *absState);
void updateTirePressureState(Message *currentMessage, TirePressure *tirePressureState);
int isValidRange(uint val1, uint val2, uint maxVal1, uint maxVal2);
int isValidPedalsPress(Message *currentMessage, Pedals *pedalsState);
int isValidSpeed(Message *currentMessage, Car *car);

//main example
int main() {
	unsigned int anomaliesIds[1000] = { 0 };
	//Enter the file name you would like to test, and call the appropriate method
	char* fileName = "1255_1256_1273_1275_1313_1316.txt";
	
	//The file name above contains only timing anomalies, hence we call detect_timing_anomalies method.
	detect_timing_anomalies(fileName, anomaliesIds);

	//Anomalies Ids detected are in anomaliesIds arr - done.
	
	return 0;
}


int detect_timing_anomalies(const char* file_path, unsigned int *anomalies_ids)
{
	FILE *file;
	file = fopen(file_path, "r");
	if (!file)
		return -1;
	
	detectAnomalies(file, anomalies_ids, TIMING_ANOMALIES);

	fclose(file);
	return 0;
}


int detect_behavioral_anomalies(const char* file_path, unsigned int *anomalies_ids)
{
	FILE *file;
	file = fopen(file_path, "r");
	if (!file)
		return -1;

	detectAnomalies(file, anomalies_ids, BEHAVIORAL_ANOMALIES);

	fclose(file);
	return 0;
}


int detect_correlation_anomalies(const char* file_path, unsigned int *anomalies_ids)
{
	FILE *file;
	file = fopen(file_path, "r");
	if (!file)
		return -1;

	detectAnomalies(file, anomalies_ids, CORRELATION_ANOMALIES);

	fclose(file);
	return 0;
}


void detectAnomalies(FILE *file, unsigned int *anomalies_ids, uint anomalyType)
{
	Message currentMessage;
	Car car;
	char line[101];
	int anomaliesArrIndex = 0;
	intialComponents(&car);

	while (fgets(line, sizeof(line), file))
	{
		if (anomaliesArrIndex == MAX_ANOMALIES)
			break;
		line[strlen(line) - 1] = '\0';
		getMessage(line, &currentMessage);

		switch (anomalyType)
		{
		case TIMING_ANOMALIES:
			if (checkForTimingAnomaly(&currentMessage, &car) == ANOMALY_DETECTED)
				anomalies_ids[anomaliesArrIndex++] = currentMessage.messageId;
			break;
		case BEHAVIORAL_ANOMALIES:
			if (checkForBehavioralAnomaly(&currentMessage, &car) == ANOMALY_DETECTED)
				anomalies_ids[anomaliesArrIndex++] = currentMessage.messageId;
			break;
		case CORRELATION_ANOMALIES:
			if (checkForCorrelationAnomaly(&currentMessage, &car) == ANOMALY_DETECTED)
				anomalies_ids[anomaliesArrIndex++] = currentMessage.messageId;
			break;
		}
	}
}

/*initalization*/
void intialComponents(Car *car)
{
	intialSpeedometer(&car->speedometer);
	intialPedals(&car->pedals);
	intialAbs(&car->abs);
	intialTirePressure(&car->tirePressure);
	car->isActive = TRUE;
}
void intialSpeedometer(Speedometer *speedometer)
{
	speedometer -> lastTransmit = 0;
	speedometer -> val1 = 0;
	speedometer -> val2 = 0;
	speedometer -> shouldAccelerate = FALSE;
	speedometer -> shouldSlowDown = FALSE;
}
void intialPedals(Pedals *pedals)
{
	pedals -> lastTransmit = 0;
	pedals->val1 = 0;
	pedals->val2 = 0;
	pedals->brakePressTime = 0;
	pedals->gasPressTime= 0;
}
void intialAbs(Abs *abs)
{
	abs->lastTransmit = 0;
	abs->val1 = 0;
	abs->val2 = 0;
}
void intialTirePressure(TirePressure *tirePressure)
{
	tirePressure -> lastTransmit = 0;
	tirePressure->val1 = 0;
	tirePressure->val2 = 100;
}

/*updates & validation*/
void getMessage(char *line, Message *currentMessage)
{
	char delimiter[2] = " ";
	char *token;
	currentMessage->messageId = atoi(strtok(line, delimiter));
	currentMessage->timestamp = atoi(strtok(NULL, delimiter));
	token = strtok(NULL, delimiter);
	sscanf(token, "%x", &currentMessage->ecuId);
	currentMessage->val1 = atoi(strtok(NULL, delimiter));
	currentMessage->val2 = atoi(strtok(NULL, delimiter));
}
void updateSpeedometerState(Message *currentMessage, Speedometer *speedometerState)
{
	speedometerState->lastTransmit = currentMessage->timestamp;
	speedometerState->val1 = currentMessage->val1;
	speedometerState->val2 = currentMessage->val2;
}
void updatePedalsState(Message *currentMessage, Pedals *pedalsState)
{
	//val1 = gas, val2 = brake
	if (pedalsState->val1 && currentMessage->val1)
		pedalsState->gasPressTime += currentMessage->timestamp - pedalsState->lastTransmit;
	else
		pedalsState->gasPressTime = 0;
	if (pedalsState->val2 && currentMessage->val2)
		pedalsState->brakePressTime += currentMessage->timestamp - pedalsState->lastTransmit;
	else
		pedalsState->brakePressTime = 0;
	pedalsState->lastTransmit = currentMessage->timestamp;
	pedalsState->val1 = currentMessage->val1;
	pedalsState->val2 = currentMessage->val2;
}
void updateAbsState(Message *currentMessage, Abs *absState)
{
	absState->lastTransmit = currentMessage->timestamp;
	absState->val1 = currentMessage->val1;
	absState->val2 = currentMessage->val2;
}
void updateTirePressureState(Message *currentMessage, TirePressure *tirePressureState)
{
	tirePressureState->lastTransmit = currentMessage->timestamp;
	tirePressureState->val1 = currentMessage->val1;
	tirePressureState->val2 = currentMessage->val2;
}
int isValidRange(uint val1, uint val2, uint maxVal1, uint maxVal2)
{
	if (val1 < 0 || val1 > maxVal1 || val2 < 0 || val2 > maxVal2)
		return FALSE;
	return TRUE;
}
int isValidPedalsPress(Message *currentMessage, Pedals *pedalsState)
{
	uint msDif = currentMessage->timestamp - pedalsState->lastTransmit;
	if (currentMessage->val1 && currentMessage->val2)
		return FALSE;
	if (pedalsState->val1)
	{
		if (!currentMessage->val1 && pedalsState->gasPressTime + msDif < 10)
		{
			return FALSE;
		}
	}
	if (pedalsState->val2)
	{

		if (!currentMessage->val2 && pedalsState->brakePressTime + msDif < 10)
		{
			return FALSE;
		}
	}
	return TRUE;
}
int isValidSpeed(Message *currentMessage, Car *car)
{
	if (!car->isActive && currentMessage->val2 != 0)
		return FALSE;

	uint msDif = currentMessage->timestamp - car->speedometer.lastTransmit;
	uint maxSpeedChangeAllow = (uint)(msDif / SPEEDOMETER_FREQUENCY) * 5;
	if (abs(currentMessage->val2 - car->speedometer.val2) > maxSpeedChangeAllow)
	{
		//case of car crush
		if (currentMessage->val2 == 0)
			car->isActive = FALSE;
		else
			return FALSE;
	}

	return TRUE;
}

/*anomalies*/
int checkForTimingAnomaly(Message *currentMessage, Car *car)
{
	int res = NO_ANOMALY_DETECTED;
	if (currentMessage->ecuId == SPEEDOMETER_ID)
	{
		if (currentMessage->timestamp != 0 && currentMessage->timestamp - car->speedometer.lastTransmit < SPEEDOMETER_FREQUENCY)
			res = ANOMALY_DETECTED;
		updateSpeedometerState(currentMessage, &car->speedometer);
	}
	else if (currentMessage->ecuId == PEDALS_ID)
	{
		if (currentMessage->timestamp != 0 && currentMessage->timestamp - car->pedals.lastTransmit < PEDALS_FREQUENCY)
			res = ANOMALY_DETECTED;
		updatePedalsState(currentMessage, &car->pedals);
	}
	else if (currentMessage->ecuId == Abs_ID)
	{
		if (currentMessage->timestamp != 0 && currentMessage->timestamp - car->abs.lastTransmit < ABS_FREQUENCY)
			res = ANOMALY_DETECTED;
		updateAbsState(currentMessage, &car->abs);
	}
	else if (currentMessage->ecuId == TIRE_PRESSURE_ID)
	{
		if (currentMessage->timestamp != 0 && currentMessage->timestamp - car->tirePressure.lastTransmit < TIRE_PRESSURE_FREQUENCY)
			res = ANOMALY_DETECTED;
		updateTirePressureState(currentMessage, &car->tirePressure);
	}
	return res;
}
int checkForBehavioralAnomaly(Message *currentMessage, Car *car)
{
	int res = NO_ANOMALY_DETECTED;
	if (currentMessage->ecuId == SPEEDOMETER_ID)
	{
		if (!isValidRange(currentMessage->val1, currentMessage->val2, FIXED_VALUE, MAX_SPEEDOMETER_VAL))
			res = ANOMALY_DETECTED;
		if (!isValidSpeed(currentMessage, car))
			res = ANOMALY_DETECTED;
		updateSpeedometerState(currentMessage, &car->speedometer);
	}
	else if (currentMessage->ecuId == PEDALS_ID)
	{
		if (!isValidRange(currentMessage->val1, currentMessage->val2, MAX_PEDALS_VAL, MAX_PEDALS_VAL))
			res = ANOMALY_DETECTED;
		if (!isValidPedalsPress(currentMessage, &car->pedals))
			res = ANOMALY_DETECTED;
		updatePedalsState(currentMessage, &car->pedals);
	}
	else if (currentMessage->ecuId == Abs_ID)
	{
		if (!isValidRange(currentMessage->val1, currentMessage->val2, FIXED_VALUE, MAX_ABS_VAL))
			res = ANOMALY_DETECTED;
		updateAbsState(currentMessage, &car->abs);
	}
	else if (currentMessage->ecuId == TIRE_PRESSURE_ID)
	{
		if (!isValidRange(currentMessage->val1, currentMessage->val2, FIXED_VALUE, MAX_TIRE_PRESSURE_VAL))
			res = ANOMALY_DETECTED;
		updateTirePressureState(currentMessage, &car->tirePressure);
	}
	return res;
}
int checkForCorrelationAnomaly(Message *currentMessage, Car *car)
{
	int res = NO_ANOMALY_DETECTED;
	if (currentMessage->ecuId == SPEEDOMETER_ID)
	{
		if (car->speedometer.shouldAccelerate && currentMessage->val2 < car->speedometer.val2)
			res = ANOMALY_DETECTED;
		if (car->speedometer.shouldSlowDown && currentMessage->val2 > car->speedometer.val2)
			res = ANOMALY_DETECTED;
		if (car->tirePressure.val2 < 30 && currentMessage->val2 > 50)
			res = ANOMALY_DETECTED;
		updateSpeedometerState(currentMessage, &car->speedometer);
	}
	else if (currentMessage->ecuId == PEDALS_ID)
	{
		if (currentMessage->val1)
		{
			car->speedometer.shouldAccelerate = TRUE;
			car->speedometer.shouldSlowDown = FALSE;
		}
		else if (currentMessage->val2) 
		{
			if (currentMessage->val2 >= HARD_PRESS)
				car->abs.val2 = MAX_ABS_VAL;
			else
				car->abs.val2 = FIXED_VALUE;
			car->speedometer.shouldSlowDown = TRUE;
			car->speedometer.shouldAccelerate = FALSE;
		}
		updatePedalsState(currentMessage, &car->pedals);
	}
	else if (currentMessage->ecuId == Abs_ID)
	{
		if (car->pedals.val2 >= HARD_PRESS && currentMessage->val2 != MAX_ABS_VAL)
			res = ANOMALY_DETECTED;
		updateAbsState(currentMessage, &car->abs);
	}
	else if (currentMessage->ecuId == TIRE_PRESSURE_ID)
	{
		if (currentMessage->val2 < 30 && car->speedometer.val2 > 50)
			res = ANOMALY_DETECTED;
		if (currentMessage->val2 > car->tirePressure.val2 && car->speedometer.val2 != 0)
			res = ANOMALY_DETECTED;
		updateTirePressureState(currentMessage, &car->tirePressure);
	}
	return res;
}
