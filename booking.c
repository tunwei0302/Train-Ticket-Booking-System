#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <Windows.h>
#include <math.h>
#include <ctype.h>
#pragma warning (disable:4996)

#define RED(string) "\x1b[31m" string "\x1b[0m"
#define GREEN(string) "\x1b[32m" string "\x1b[0m"
#define BLUE(string) "\x1b[34m" string "\x1b[0m"
#define WHITE(string) "\x1b[37m" string "\x1b[0m"
#define PAX_LIMIT 6
#define MAX_STATION 15
#define MAX_COACH 10
#define MAX_SEAT 20
#define MAX_STREETNAMESTAFF 30
#define MAX_RESIDENTAREASTAFF 30
#define MAX_CITYSTAFF 20
#define MAX_IDSTAFF 7
#define MAX_NAMESTAFF 25
#define MAX_POSITIONSTAFF 20
#define MAX_PASSWORDSTAFF 30
#define MAX_STRINGSTAFF 30
#define MAX_FORGOTPASSSTAFF 30

#define MAX_ID_LENGTH 6
#define MAX_NAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 20
#define MAX_GENDER_LENGTH 7
#define MAX_IC_LENGTH 15
#define MAX_CONTACT_LENGTH 15
#define MAX_SECURITY_QUESTION_LENGTH 100
#define MAX_SECURITY_ANSWER_LENGTH 100
#define MAX_CHOICE_LENGTH 6

typedef struct {
	int houseNum;
	char streetName[MAX_STREETNAMESTAFF];
	char residentArea[MAX_RESIDENTAREASTAFF];
	char city[MAX_CITYSTAFF];
}Address;

typedef struct {
	int num;
	char id[MAX_IDSTAFF];
	char name[MAX_NAMESTAFF];
	char position[MAX_POSITIONSTAFF];
	char password[MAX_PASSWORDSTAFF];
	char forgotPass[MAX_FORGOTPASSSTAFF];
	int forgotPassQues;
	Address staffAddress;
}Staff;

typedef struct {
	int day;
	int month;
	int year;
	int hour;
	int minute;
}logDate;

// Schedulu Module Structure
typedef struct {
	int day;
	int month;
}Date;

typedef struct {
	Date date[7];
	char status[12];
	int seatNumber;
}Seat;

typedef struct {
	char coachNumber;
	Seat seat[MAX_SEAT];
}Coach;

typedef struct {
	Coach coach[MAX_COACH];
	char name[30];
	int arrivalHour, arrivalMinute;
}Station;

typedef struct {
	Station station[MAX_STATION];
	int trainNumber;
}Train;

// ***********************************************************

// Booking Module Structure
typedef struct {
	char paxName[20];
	char paxIC[13];
}PaxDetail;

typedef struct {
	char paymentMethod[20];
	double amount;
	double totalAmount;
}Payment;

typedef struct {
	char departureStation[30];
	char arrivalStation[30];
	int departureMonth;
	int departureDay;
	int departureHour;
	int departureMinute;
	int arrivalHour;
	int arrivalMinute;
}StationDetail;

typedef struct {
	char coach;
	int seatNumber;
}SeatDetail;

typedef struct {
	int trainID;
	SeatDetail seatDetail[PAX_LIMIT];
}TrainDetail;

typedef struct {
	int bYear;
	int bMonth;
	int bDay;
}BookingDate;

typedef struct {
	char ticketID[13];
	char memberID[13];
	int pax;
	PaxDetail pDetail[PAX_LIMIT];
	Payment payment;
	StationDetail sDetail;
	TrainDetail tDetail;
	BookingDate bDate;
	char ticketStatus[20];
}Ticket;

struct Member {
	char id[MAX_ID_LENGTH];
	char name[MAX_NAME_LENGTH];
	char password[MAX_PASSWORD_LENGTH];
	char passwordRecovery[MAX_PASSWORD_LENGTH];
	char gender[MAX_GENDER_LENGTH];
	char IC[MAX_IC_LENGTH];
	char contact[MAX_CONTACT_LENGTH];
	char securityQuestion1[MAX_SECURITY_QUESTION_LENGTH];
	char securityAnswer1[MAX_SECURITY_ANSWER_LENGTH];
};

// ***********************************************************

int validateStation(int option);
int availableSeat(int departureStation, int arrivalStation, Train* ptrTrain);
double calculatePrice(int distance);
int vName(int i, char paxName[20]);
int vIC(int i, char paxIC[12]);

void updateValidDate();
void getCurrentDate(int* month, int* day, int m);
int getCurrentTime(int hour, int minute);
int validateDate(int day, int month, Train* ptrTrain);
void sStation(Train* ptrTrain);

void previewTicket(Ticket* ptrTicket, struct Member* member);
void showInvoice(Ticket* ptrTicket);
void viewSummaryTicket(Ticket* ptrTicket);
void viewTicket(Ticket* ptrTicket);

void updateTicket();
void updateTicketLog(char ticID[13], char ticketStatus[20], Staff* pStaffInfo);
// ***********************************************************

// Booking Module Function
void memberBooking(struct Member* member);
void declareTicID_bDate(Ticket* ptrTicket, struct Member* member);
void selectStation(Ticket* ptrTicket, char* departureStation[30], char* arrivalStation[30]);
int estimateTime(Ticket* ptrTicket, struct Member* member, char* departure[30], char* arrival[30]);
void selectDepTime_ArrTime(Ticket* ptrTicket, int departureStation, int arrivalStation, int x, int hold[][1]);
void selectPax(Ticket* ptrTicket);
void addPaxDetail(Ticket* ptrTicket, int pax);
void selectSeat(Ticket* ptrTicket, int departureStation, int arrivalStation);
int showCoach_Seat(Train* ptrTrain, int departureStation, int arrivalStation, int y, int coach);
void paymentProcess(Ticket* ptrTicket, struct Member* member);

void viewTicMenu(struct Member* member);
void checkByTicID(struct Member* member);
void checkByMemID(struct Member* member);
void checkValidTic(struct Member* member);

void editTicMenu(Staff* pStaffInfo);
void editTicket(Ticket* ptrTicket, char ticID[13], Staff* pStaffInfo);
void deleteTicket(char ticID[13], Staff* pStaffInfo);
void editByTicID(Staff* pStaffInfo);
void editByDerpatureDate_TrainID(Staff* pStaffInfo);
void massEditTic(int month, int day, int trainID, Staff* pStaffInfo);
void massDeleteTic(int month, int day, int trainID, Staff* pStaffInfo);

// ********************************************************************************************************************************
void memberBooking(struct Member* member) {

	int x = 1;

	Train train;
	Ticket ticket;


	updateTicket();


	char departureS[30], arrivalS[30];

	int opt, valid = -1;

	declareTicID_bDate(&ticket, member);

	selectStation(&ticket, departureS, arrivalS);

	x = estimateTime(&ticket, member, departureS, arrivalS);

	if (x) {
		system("cls");
		previewTicket(&ticket, member);

		do {
			printf("\n[Double Check]\n");
			printf("1. Continue To Payment\n2. Return to Menu\nSelect: ");
			scanf("%d", &opt);
			rewind(stdin);

			if (opt == 1)
				paymentProcess(&ticket, member);
			else if (opt == 2)
				;
			else {
				printf("\nInput Error, Please Re-Enter!!!\n\n");
				Sleep(1500);
			}
		} while (opt != 1 && opt != 2);
	}
}
void declareTicID_bDate(Ticket* ptrTicket, struct Member* member) {

	int year, month, day, tempID, loop;
	char tempTicID[13];

	Ticket tempTic;

	SYSTEMTIME t;
	GetLocalTime(&t);

	year = t.wYear;
	month = t.wMonth;
	day = t.wDay;

	FILE* booking = fopen("ticket.dat", "rb");

	do {
		loop = 0;

		srand(time(NULL));
		tempID = 1000 + rand() % 9000;
		sprintf(tempTicID, "%04d%02d%02d%04d", year, month, day, tempID);

		fread(&tempTic, sizeof(Ticket), 1, booking);
		if (strcmp(tempTicID, tempTic.ticketID) != 0) {
			while (!feof(booking)) {
				fread(&tempTic, sizeof(Ticket), 1, booking);
				if (strcmp(tempTicID, tempTic.ticketID) == 0) {
					loop = 1;
				}
			}
		}
		else if (strcmp(tempTicID, tempTic.ticketID) == 0) {
			loop = 1;
		}
	} while (loop);

	fclose(booking);

	ptrTicket->bDate.bYear = t.wYear;
	ptrTicket->bDate.bMonth = t.wMonth;
	ptrTicket->bDate.bDay = t.wDay;

	strcpy(ptrTicket->ticketID, tempTicID);
	strcpy(ptrTicket->memberID, member->id);
}
void selectStation(Ticket* ptrTicket, char* departureStation[30], char* arrivalStation[30]) {
	Train train;
	FILE* schedule = fopen("schedule.dat", "rb");

	fread(&train, sizeof(Train), 1, schedule);

	int month, day, opt;

	do {
		system("cls");
		printf("Booking Can Only Be Done From %02d/%02d To %02d/%02d\n\n", train.station[0].coach[0].seat[0].date[0].month, train.station[0].coach[0].seat[0].date[0].day,
			train.station[0].coach[0].seat[0].date[6].month, train.station[0].coach[0].seat[0].date[6].day); // Limit User To Book Only In 7 Days
		printf("[Departure Date]\n");
		printf("Enter Departure Date (mm/dd): ");
		scanf("%d/%d", &month, &day);
		rewind(stdin);

		if (validateDate(day, month, &train) == 1) { //Validate The Date Entered By User
			printf("\nInvalid Date, Please Re-Enter!!!\n\n");
			Sleep(1500);
		}


	} while (validateDate(day, month, &train) == 1);
	ptrTicket->sDetail.departureDay = day;
	ptrTicket->sDetail.departureMonth = month;

	do {
		printf("\n\t   [Departure Station]\n");
		sStation(&train); //List All The Station

		printf("\nSelect Station(Number): ");
		scanf("%d", &opt);
		rewind(stdin);

		if (validateStation(opt) == 1) {
			printf("\nInput Error, Please Re-Enter!!!\n");
			Sleep(1500);
		}
		else {
			strcpy(departureStation, train.station[opt - 1].name);
		}
	} while (validateStation(opt) == 1);
	printf("From: %s\n", departureStation);
	strcpy(ptrTicket->sDetail.departureStation, departureStation);

	do {
		printf("\n\t   [Arrival Station]\n");
		sStation(&train);

		printf("\nSelect Station(Number): ");
		scanf("%d", &opt);
		rewind(stdin);

		if (validateStation(opt) == 1) {
			printf("\nInput Error, Please Re-Enter!!!\n");
			Sleep(1500);
		}
		else {
			strcpy(arrivalStation, train.station[opt - 1].name);
		}

		if (strcmp(arrivalStation, departureStation) == 0) {
			printf("\nSame Station, Please Re-Enter!!!\n");
			Sleep(1500);
		}
	} while (validateStation(opt) == 1 || strcmp(arrivalStation, departureStation) == 0);
	printf("To: %s\n", arrivalStation);
	strcpy(ptrTicket->sDetail.arrivalStation, arrivalStation);
}

int estimateTime(Ticket* ptrTicket, struct Member* member, char* departure[30], char* arrival[30]) {
	Train train;

	FILE* schedule = fopen("schedule.dat", "rb");
	fread(&train, sizeof(Train), 1, schedule);
	rewind(schedule);

	int/* temp = 0, done = -2,*/distance, /*valid = -1,*/ x = 0, option;
	char d[30], a[30];
	int departureStation, arrivalStation;
	int i = 0;
	int hold[50][1];

	option = 0;

	for (int i = 0; i < MAX_STATION; i++) {
		if (strcmp(train.station[i].name, departure) == 0) {
			departureStation = i;
		}
		if (strcmp(train.station[i].name, arrival) == 0) {
			arrivalStation = i;
		}
	}
	distance = abs(arrivalStation - departureStation);

	system("cls");
	printf("\n[Departure Time]\n");
	printf("From: %s\n\n", train.station[departureStation].name);
	printf("To: %s\n", train.station[arrivalStation].name);
	printf(" No.  Train ID      Departure Time\tArrival Time\tAvailable Seat\n");
	printf("=============================================================================\n");
	while (fread(&train, sizeof(Train), 1, schedule) != 0) {
		if (train.station[departureStation].arrivalHour == 0 && train.station[departureStation].arrivalMinute != 0) {
			train.station[departureStation].arrivalHour = 24;
		}
		if (train.station[arrivalStation].arrivalHour == 0 && train.station[arrivalStation].arrivalMinute != 0) {
			train.station[arrivalStation].arrivalHour = 24;
		}
		if (((train.station[departureStation].arrivalHour * 60) + train.station[departureStation].arrivalMinute) < ((train.station[arrivalStation].arrivalHour * 60) + train.station[arrivalStation].arrivalMinute)) {
			if ((train.station[departureStation].arrivalHour != 0 || train.station[departureStation].arrivalMinute != 0) && (train.station[arrivalStation].arrivalHour != 0 || train.station[arrivalStation].arrivalMinute != 0)) {
				if (train.station[departureStation].arrivalHour == 24 && train.station[departureStation].arrivalMinute != 0) {
					train.station[departureStation].arrivalHour = 0;
				}
				if (train.station[arrivalStation].arrivalHour == 24 && train.station[arrivalStation].arrivalMinute != 0) {
					train.station[arrivalStation].arrivalHour = 0;
				}
				printf(" %2d   %-14d%7.02d:%02d %12.02d:%02d%16d\n", (x + 1), train.trainNumber, train.station[departureStation].arrivalHour, train.station[departureStation].arrivalMinute,
					train.station[arrivalStation].arrivalHour, train.station[arrivalStation].arrivalMinute, availableSeat(departureStation, arrivalStation, &train));
				hold[x][0] = i;
				x++;

			}
		}
		i++;
	}

	printf("=============================================================================\n");
	printf("PRICE : RM%.2f\n\n", calculatePrice(distance));
	ptrTicket->payment.amount = calculatePrice(distance);
	do {
		printf("\n1. Menu\n");
		printf("2. Make Booking\n");
		printf("Select > ");
		scanf("%d", &option);
		rewind(stdin);

		switch (option) {
		case 1:
			return 0;
			break;
		case 2:
			selectDepTime_ArrTime(ptrTicket, departureStation, arrivalStation, x, hold);
			selectPax(ptrTicket);
			selectSeat(ptrTicket, departureStation, arrivalStation);
			return 1;
			break;
		default:
			printf("Invalid Option!!!\n");
			Sleep(1500);
		}
	} while (option != 1 && option != 2);

}
void selectDepTime_ArrTime(Ticket* ptrTicket, int departureStation, int arrivalStation, int x, int hold[][1]) {

	Train train;

	FILE* schedule = fopen("schedule.dat", "rb");

	int opt, i = 0;

	do {
		printf("\nPlease select your departure time: ");
		scanf("%d", &opt);
		rewind(stdin);

		if (opt < 1 || opt > x) {
			printf("\nInput Error, Please Re-Enter!!!\n\n");
			Sleep(1500);
		}
	} while (opt < 1 || opt > x);

	while (fread(&train, sizeof(Train), 1, schedule) != 0) {

		if (hold[opt - 1][0] == i) {//value of x is fileloop for train x is num of selection
			break;
		}
		i++;
	}
	ptrTicket->tDetail.trainID = train.trainNumber;
	ptrTicket->sDetail.departureHour = train.station[departureStation].arrivalHour;
	ptrTicket->sDetail.departureMinute = train.station[departureStation].arrivalMinute;
	ptrTicket->sDetail.arrivalHour = train.station[arrivalStation].arrivalHour;
	ptrTicket->sDetail.arrivalMinute = train.station[arrivalStation].arrivalMinute;


	fclose(schedule);

}
void selectPax(Ticket* ptrTicket) {

	int pax;

	do {
		printf("\n[Number of Pax]\n");
		printf("PLEASE Enter number of pax: ");
		scanf("%d", &pax);
		rewind(stdin);

		if (pax < 1) {
			printf("\nPlease select at least 1 pax!!!\n\n");
			Sleep(1500);
		}
		else if (pax > PAX_LIMIT) {
			printf("\nPax surpass the maximum limit of 6!!!\n\n");
			Sleep(1500);
		}
	} while (pax < 1 || pax > PAX_LIMIT);
	ptrTicket->pax = pax;

	addPaxDetail(ptrTicket, pax);
}
void addPaxDetail(Ticket* ptrTicket, int pax) {

	system("cls");
	for (int i = 0; i < pax; i++) {
		do {
			do {
				printf("\n[Passenger %d]\n", (i + 1));
				printf("Name: ");
				scanf("%[^\n]", ptrTicket->pDetail[i].paxName);
				rewind(stdin);

				if (vName(i, ptrTicket->pDetail[i].paxName) == 1) {
					printf("Name exceed the 20 maximum limit!!!\n");
					Sleep(1500);
				}
				else if (vName(i, ptrTicket->pDetail[i].paxName) == 2) {
					printf("Name need at least 3 character!!!\n");
					Sleep(1500);
				}
				else if (vName(i, ptrTicket->pDetail[i].paxName) == 3) {
					printf("Name cannot contain non-alphabet character!!!\n");
					Sleep(1500);
				}
			} while (vName(i, ptrTicket->pDetail[i].paxName));

			do {
				printf("\nIC Number (xxxxxxxxxxxx)(-1 to go back): ");
				scanf("%[^\n]", ptrTicket->pDetail[i].paxIC);
				rewind(stdin);

				if (strcmp(ptrTicket->pDetail[i].paxIC, "-1") == 0) {
					break;
				}
				else if (vIC(i, ptrTicket->pDetail[i].paxIC) == 1) {
					printf("IC number must have 12 number\n");
					Sleep(1500);
				}
				else if (vIC(i, ptrTicket->pDetail[i].paxIC) == 2) {
					printf("IC number cannot contain non-digit character!!!\n");
					Sleep(1500);
				}
			} while (vIC(i, ptrTicket->pDetail[i].paxIC));

		} while (strcmp(ptrTicket->pDetail[i].paxIC, "-1") == 0);
	}
}
void selectSeat(Ticket* ptrTicket, int departureStation, int arrivalStation) {
	Train train;

	FILE* scheduleOut = fopen("schedule.dat", "rb");
	FILE* tmp = tmpfile();

	int coach = 0, i = 0;
	int seat, m, k;


	while (fread(&train, sizeof(Train), 1, scheduleOut) != 0) {
		if (ptrTicket->tDetail.trainID == train.trainNumber) {
			break;
		}
		fwrite(&train, sizeof(Train), 1, tmp);
	}

	for (int y = 0; y < ptrTicket->pax; y++) {
		do {
			coach = showCoach_Seat(&train, departureStation, arrivalStation, y, coach);
			do {
				m = 0;
				do {
					printf("\n[Select Seat]\n");
					printf("Select your seat (-1 to go back): %c", train.station[0].coach[coach].coachNumber);
					scanf("%d", &seat);
					rewind(stdin);

					if (seat == -1) {
						break;
					}
					else if (seat < 1 || seat > 20) {
						printf("\nInput Error!! Please Select 1 - 20\n");
						Sleep(1500);
					}
				} while (seat < 1 || seat > 20);

				for (k = 0; k < 20; k++) {
					if (departureStation < arrivalStation) {
						for (i = departureStation; i < arrivalStation; i++) {
							if (seat == train.station[i].coach[coach].seat[k].seatNumber) {
								if (strcmp(train.station[i].coach[coach].seat[k].status, "Available") != 0) {
									m = -1;
								}
							}
						}
					}
					else if (departureStation > arrivalStation) {
						for (i = departureStation; i > arrivalStation; i--) {
							if (seat == train.station[i].coach[coach].seat[k].seatNumber) {
								if (strcmp(train.station[i].coach[coach].seat[k].status, "Available") != 0) {
									m = -1;
								}
							}
						}
					}
				}

				if (m == -1) {
					printf("\nThe seat is booked, Please select again!!\n");
					Sleep(1500);
				}
			} while (m);
		} while (seat == -1);

		for (k = 0; k < 20; k++) {
			if (seat == train.station[departureStation].coach[coach].seat[k].seatNumber) {
				ptrTicket->tDetail.seatDetail[y].seatNumber = train.station[0].coach[coach].seat[k].seatNumber;
				ptrTicket->tDetail.seatDetail[y].coach = train.station[0].coach[coach].coachNumber;
				if (departureStation < arrivalStation) {
					for (i = departureStation; i < arrivalStation; i++) {
						strcpy(train.station[i].coach[coach].seat[k].status, "Booked");
					}
				}
				else if (departureStation > arrivalStation) {
					for (i = departureStation; i > arrivalStation; i--) {
						strcpy(train.station[i].coach[coach].seat[k].status, "Booked");
					}
				}
			}
		}
	}
	fwrite(&train, sizeof(Train), 1, tmp);

	while (fread(&train, sizeof(Train), 1, scheduleOut) != 0) {
		fwrite(&train, sizeof(Train), 1, tmp);
	}
	fclose(scheduleOut);
	rewind(tmp);

	FILE* scheduleIn = fopen("schedule.dat", "wb");

	while (fread(&train, sizeof(Train), 1, tmp) != 0) {
		fwrite(&train, sizeof(Train), 1, scheduleIn);
	}
	fclose(scheduleIn);
	fclose(tmp);

}
int showCoach_Seat(Train* ptrTrain, int departureStation, int arrivalStation, int y, int coach) {

	int valid, k;
	int	x = 1;
	int i;

	do {
		printf("\n[Passenger %d]\n", (y + 1));
		printf("[Coach]\n");
		printf("Please Select a Coach: ");
		scanf("%d", &coach);
		rewind(stdin);

		if (coach < 1 || coach > 10) {
			printf("\nInput Error!! Please Select 1 - 10\n");
			Sleep(1500);
		}
	} while (coach < 1 || coach > 10);

	printf("\n[Coach %d]\n", coach);
	coach--;

	for (k = 0; k < 20; k++) {
		valid = 0;
		if (departureStation < arrivalStation) {
			for (i = departureStation; i < arrivalStation; i++) {
				if (strcmp(ptrTrain->station[i].coach[coach].seat[k].status, "Available") != 0) {
					valid = -1;
				}
			}
			if (valid == 0) {
				printf("\033[0;32m"); //Green
				printf(" %c%02d  ", ptrTrain->station[i].coach[coach].coachNumber, ptrTrain->station[i].coach[coach].seat[k].seatNumber);
				x++;
				if (x > 10) {
					printf("\n");
					x = 0;
				}
			}
			else {
				printf("\033[0;31m"); //Red
				printf(" %c%02d  ", ptrTrain->station[i].coach[coach].coachNumber, ptrTrain->station[i].coach[coach].seat[k].seatNumber);
				x++;
				if (x > 10) {
					printf("\n");
					x = 0;
				}
			}
		}
		else if (departureStation > arrivalStation) {
			for (i = departureStation; i > arrivalStation; i--) {
				if (strcmp(ptrTrain->station[i].coach[coach].seat[k].status, "Available") != 0) {
					valid = -1;
				}
			}
			if (valid == 0) {
				printf("\033[0;32m");
				printf(" %c%02d  ", ptrTrain->station[i].coach[coach].coachNumber, ptrTrain->station[i].coach[coach].seat[k].seatNumber);
				x++;
				if (x > 10) {
					printf("\n");
					x = 0;
				}
			}
			else {
				printf("\033[0;31m");
				printf(" %c%02d  ", ptrTrain->station[i].coach[coach].coachNumber, ptrTrain->station[i].coach[coach].seat[k].seatNumber);
				x++;
				if (x > 10) {
					printf("\n");
					x = 0;
				}
			}
		}
	}
	printf("\033[0m");
	return coach;
}
void paymentProcess(Ticket* ptrTicket, struct Member* member) {

	int opt;
	do {
		do {
			printf("\n[Payment Method]\n");
			printf("1. Debit/Credit Card \n2. Touch 'n Go\n");
			printf("Select Payment Method: ");
			scanf("%d", &opt);
			rewind(stdin);

			if (opt == 1) {
				strcpy(ptrTicket->payment.paymentMethod, "Debit/Credit Card");
			}
			else if (opt == 2) {
				strcpy(ptrTicket->payment.paymentMethod, "Touch 'n Go");
			}
			else {
				printf("\nInput Error, Please Re-Enter!!!\n");
				Sleep(1500);
			}
		} while (opt != 1 && opt != 2);

		showInvoice(ptrTicket);

		printf("\n[Confirm Payment?]\n");
		printf("1. Make payment\n2. Back\n3. Back to Menu\n");
		printf("Select: ");
		scanf("%d", &opt);
		rewind(stdin);

		if (opt == 1) {
			strcpy(ptrTicket->ticketStatus, "Valid");
		}
		else if (opt == 2 || opt == 3) {
			;
		}
		else {
			printf("\nInput Error, Please Re-Enter!!!\n");
			Sleep(1500);
		}
	} while (opt != 1 && opt != 3);

	if (opt == 1) {
		printf("\nProcessing......\n");
		getch();

		FILE* booking = fopen("ticket.dat", "ab");
		if (booking == NULL) {
			printf("Error!!! Unable to open the file\n");
			exit(-1);
		}
		fwrite(ptrTicket, sizeof(Ticket), 1, booking);
		fclose(booking);

		printf("\nPayment Successfull\n");
		getch();
	}

	updateSeat();
}

void viewTicMenu(struct Member* member) {

	int opt;

	do {
		system("cls");
		printf("[View Ticket Menu]\n");
		printf("1. View All Ticket\n2. View Valid Ticket\n3. Main Menu\n");
		printf("Select: ");
		scanf("%d", &opt);
		rewind(stdin);

		if (opt == 1) {
			checkByMemID(member);
		}
		else if (opt == 2) {
			checkValidTic(member);
		}
		else if (opt == 3) {
			break;
			/*checkSchedule(member);*/
		}
		else {
			printf("\nInput Error!! Please select 1 - 4\n");
			Sleep(1500);
		}
	} while (opt != 4);

}
void checkByTicID(struct Member* member) {

	Ticket tempTicket;

	int found = 1;
	char tempTicID[13];
	do {
		printf("\nEnter Ticket ID to view (-1 to return to View Ticket Menu): ");
		scanf("%[^\n]", tempTicID);
		rewind(stdin);

		if (strcmp(tempTicID, "-1") == 0) {
			printf("\n");
			break;
		}

		FILE* checking = fopen("ticket.dat", "rb");
		if (checking == NULL) {
			printf("Error!!! Unable to open the file\n");
			exit(-1);
		}

		while (fread(&tempTicket, sizeof(Ticket), 1, checking)) {
			if (strcmp(tempTicket.ticketID, tempTicID) == 0) {
				viewTicket(&tempTicket);
				getch();
				found = 0;
			}
		}
		fclose(checking);

		if (found) {
			printf("\nTICKET NOT FOUND!!!\n");
			Sleep(1500);
		}
	} while (found);
}
void checkByMemID(struct Member* member) {

	Ticket tempTicket;

	int opt, count = 1, found = 1;

	FILE* checking = fopen("ticket.dat", "rb");
	if (checking == NULL) {
		printf("Error!!! Unable to open the file\n");
		exit(-1);
	}
	while (fread(&tempTicket, sizeof(Ticket), 1, checking)) {
		if (strcmp(tempTicket.memberID, member->id) == 0) {
			printf("\n[%d]", count);
			viewSummaryTicket(&tempTicket);
			found = 0;
			count++;
		}
	}
	fclose(checking);

	if (found) {
		printf("\nNO RECORD FOUND!!!\n\n");
		Sleep(1500);
	}
	else {
		do {
			printf("\n1. Check Ticket By TicketID\n2. View Ticket Menu\n");
			printf("Select: ");
			scanf("%d", &opt);
			rewind(stdin);

			if (opt == 1) {
				checkByTicID(member);
			}
			else if (opt == 2) {
				printf("\n");
				break;
			}
			else {
				printf("Please select 1 or 2!!\n\n");
				Sleep(1500);
			}
		} while (opt != 1);
	}
}
void checkValidTic(struct Member* member) {

	Ticket tempTicket;

	int opt, count = 1, found = 1;

	FILE* checking = fopen("ticket.dat", "rb");
	if (checking == NULL) {
		printf("Error!!! Unable to open the file\n");
		exit(-1);
	}
	while (fread(&tempTicket, sizeof(Ticket), 1, checking)) {
		if (strcmp(tempTicket.memberID, member->id) == 0) {
			if (strcmp(tempTicket.ticketStatus, "Valid") == 0) {
				printf("\n[%d]", count);
				viewSummaryTicket(&tempTicket);
				found = 0;
				count++;
			}
		}
	}
	fclose(checking);

	if (found) {
		printf("\nNO RECORD FOUND!!!\n\n");
		Sleep(1500);
	}
	else {
		do {
			printf("\n1. Check Ticket By TicketID\n2. View Ticket Menu\n");
			printf("Select: ");
			scanf("%d", &opt);
			rewind(stdin);

			if (opt == 1) {
				checkByTicID(member);
			}
			else if (opt == 2) {
				printf("\n");
				break;
			}
			else {
				printf("Please select 1 or 2!!\n\n");
				Sleep(1500);
			}
		} while (opt != 1);
	}
}

void editTicMenu(Staff* pStaffInfo) {

	int opt;

	do {
		system("cls");
		printf("\n[Edit Ticket Menu]\n");
		printf("1. Edit By TicketID\n2. Edit in Sum(DepatureDate & TrainID)\n3. Back to Main Menu\n");
		printf("Select: ");
		scanf("%d", &opt);
		rewind(stdin);

		if (opt == 1)
			editByTicID(pStaffInfo);
		else if (opt == 2)
			editByDerpatureDate_TrainID(pStaffInfo);
		else if (opt == 3)
			;
		else {
			printf("\nInput Error!! Please select 1 - 3\n");
			Sleep(1500);
		}
	} while (opt != 3);
}
void editTicket(Ticket* ptrTicket, char ticID[13], Staff* pStaffInfo) {

	Ticket tempTicket1;

	tempTicket1 = *ptrTicket;

	int opt;
	char opt2 = 'Y';
	char tempTicID[13];
	strcpy(tempTicID, ticID);

	do {
		printf("1. Valid\n2. Invalid\n3. Canceled\n4. Delete\n5. Edit Ticket Menu\n");
		printf("Edit Ticket Status: ");
		scanf("%d", &opt);
		rewind(stdin);

		if (opt < 0 || opt > 5) {
			printf("Input Error!! Please re-Select!!\n");
			Sleep(1500);
		}

		if (opt == 4) {
			printf("\nAre you SURE!! THE RECORD SELECTED WILL BE DELETED!!! (Y/N) \n");
			scanf("%c", &opt2);
			rewind(stdin);

			if (toupper(opt2) != 'Y') {
				printf("\nReturning\n\n");
				Sleep(1500);
			}
		}
	} while ((opt < 0 || opt > 5) || toupper(opt2) != 'Y');

	if (opt == 1) {
		strcpy(tempTicket1.ticketStatus, "Valid");
		updateTicketLog(ticID, tempTicket1.ticketStatus, pStaffInfo);
	}
	else if (opt == 2) {
		strcpy(tempTicket1.ticketStatus, "Invalid");
		updateTicketLog(ticID, tempTicket1.ticketStatus, pStaffInfo);
	}
	else if (opt == 3) {
		strcpy(tempTicket1.ticketStatus, "Canceled");
		updateTicketLog(ticID, tempTicket1.ticketStatus, pStaffInfo);
	}
	else if (opt == 4) {
		deleteTicket(tempTicID, pStaffInfo);
		printf("Delete Succesful!!\n\n");
		Sleep(1500);
	}

	if (opt == 1 || opt == 2 || opt == 3) {
		FILE* viewing = fopen("ticket.dat", "rb");
		FILE* editing = fopen("tempTicHolder.dat", "ab");
		fwrite(&tempTicket1, sizeof(Ticket), 1, editing);
		while (fread(&tempTicket1, sizeof(Ticket), 1, viewing)) {
			if (strcmp(tempTicID, tempTicket1.ticketID) != 0) {
				fwrite(&tempTicket1, sizeof(Ticket), 1, editing);
			}
		}
		fclose(viewing);
		fclose(editing);

		FILE* clear = fopen("ticket.dat", "wb");
		fclose(clear);

		FILE* view = fopen("tempTicHolder.dat", "rb");
		FILE* edit = fopen("ticket.dat", "ab");
		while (fread(&tempTicket1, sizeof(Ticket), 1, view)) {
			fwrite(&tempTicket1, sizeof(Ticket), 1, edit);
		}
		fclose(view);
		fclose(edit);

		FILE* clear2 = fopen("tempTicHolder.dat", "wb");
		fclose(clear2);
	}
}
void deleteTicket(char ticID[13], Staff* pStaffInfo) {

	Ticket tempTicket;

	char tempTicID[13];
	strcpy(tempTicID, ticID);

	FILE* viewing = fopen("ticket.dat", "rb");
	FILE* editing = fopen("tempTicHolder.dat", "ab");
	while (fread(&tempTicket, sizeof(Ticket), 1, viewing)) {
		if (strcmp(tempTicID, tempTicket.ticketID) != 0) {
			fwrite(&tempTicket, sizeof(Ticket), 1, editing);
		}
	}
	fclose(viewing);
	fclose(editing);

	FILE* clear = fopen("ticket.dat", "wb");
	fclose(clear);

	FILE* view = fopen("tempTicHolder.dat", "rb");
	FILE* edit = fopen("ticket.dat", "ab");
	while (fread(&tempTicket, sizeof(Ticket), 1, view)) {
		fwrite(&tempTicket, sizeof(Ticket), 1, edit);
	}
	fclose(view);
	fclose(edit);

	FILE* clear2 = fopen("tempTicHolder.dat", "wb");
	fclose(clear2);

	logDate date;
	sGetCurrentTime(&date.day, &date.month, &date.year, &date.hour, &date.minute);

	FILE* addLog = fopen("staffLog.txt", "a");
	if (addLog == NULL) {
		printf("Unable to open staff.txt\n");
		exit(-1);
	}
	fprintf(addLog, "%s deleted Ticket ID \"%s\"|%02d-%02d-%d %02d:%02d\n", pStaffInfo->id, ticID, date.day, date.month, date.year, date.hour, date.minute);

	fclose(addLog);

}
void editByTicID(Staff* pStaffInfo) {

	Ticket tempTicket;

	int found = 1;
	char tempTicID[13];
	do {
		printf("\nEnter Ticket ID to edit (-1 to return to View Ticket Menu): ");
		scanf("%[^\n]", tempTicID);
		rewind(stdin);

		if (strcmp(tempTicID, "-1") == 0) {
			printf("\n");
			break;
		}

		FILE* checking = fopen("ticket.dat", "rb");
		if (checking == NULL) {
			printf("Error!!! Unable to open the file\n");
			exit(-1);
		}

		while (fread(&tempTicket, sizeof(Ticket), 1, checking)) {
			if (strcmp(tempTicket.ticketID, tempTicID) == 0) {
				fclose(checking);
				viewTicket(&tempTicket);
				editTicket(&tempTicket, tempTicID, pStaffInfo);
				found = 0;
			}
		}
		if (found) {
			printf("\nTICKET NOT FOUND!!!\n");
			Sleep(1500);
		}
	} while (found);
}
void editByDerpatureDate_TrainID(Staff* pStaffInfo) {

	Ticket tempTicket;

	int found = 1, count = 1, opt;
	char opt2 = 'N';
	int month, day, trainID;

	do {
		printf("\nEnter Depature Date (mm/dd) (-1 to return to View Ticket Menu) : ");
		scanf("%d/%d", &month, &day);
		rewind(stdin);

		if (month == -1) {
			printf("\n");
			break;
		}

		printf("\nEnter TrainID (-1 to return to View Ticket Menu) : ");
		scanf("%d", &trainID);
		rewind(stdin);

		if (trainID == -1) {
			printf("\n");
			break;
		}

		FILE* checking = fopen("ticket.dat", "rb");
		if (checking == NULL) {
			printf("\nError!!! Unable to open the file\n");
			exit(-1);
		}
		while (fread(&tempTicket, sizeof(Ticket), 1, checking)) {
			if (tempTicket.sDetail.departureMonth == month && tempTicket.sDetail.departureDay == day) {
				if (tempTicket.tDetail.trainID == trainID) {
					printf("\n[%d]", count);
					viewSummaryTicket(&tempTicket);
					found = 0;
					count++;
				}
			}
		}
		fclose(checking);

		if (found) {
			printf("\nNO RECORD FOUND!!!\n");
			Sleep(1500);
		}
		else {
			do {
				printf("1. Mass Edit\n2. Mass Delete\n3. Return To Edit Ticket Menu\n");
				printf("Select: ");
				scanf("%d", &opt);
				rewind(stdin);

				if (opt == 1) {
					massEditTic(month, day, trainID, pStaffInfo);
					break;
				}
				else if (opt == 2) {
					printf("\nAre you SURE!! THE RECORD SELECTED WILL BE DELETED!!! (Y/N) \n");
					scanf("%c", &opt2);
					rewind(stdin);

					if (toupper(opt2) == 'Y') {
						massDeleteTic(month, day, trainID, pStaffInfo);
						printf("\nDelete Succesful!!\n\n");
						Sleep(1500);
						break;
					}
					else {
						printf("\nReturning\n\n");
						Sleep(1500);
					}
				}
				else if (opt == 3) {
					break;
				}
				else {
					printf("\nInput Error!! Please select 1 - 3\n");
					Sleep(1500);
				}
			} while (1);
		}
	} while (found);
}
void massEditTic(int month, int day, int trainID, Staff* pStaffInfo) {

	Ticket tempTicket;

	int opt;
	char ticStatus[20];

	do {
		printf("\n1. Valid\n2. Invalid\n3. Canceled\n4. Back To Edit Ticket Menu\n");
		printf("Edit Ticket Status: ");
		scanf("%d", &opt);
		rewind(stdin);

		if (opt < 1 || opt > 4) {
			printf("Input Error!! Please re-Select!!\n");
			Sleep(1500);
		}
	} while (opt < 1 || opt > 4);

	if (opt == 1) {
		strcpy(ticStatus, "Valid");
	}
	else if (opt == 2) {
		strcpy(ticStatus, "Invalid");
	}
	else if (opt == 3) {
		strcpy(ticStatus, "Canceled");
	}

	if (opt == 1 || opt == 2 || opt == 3) {
		FILE* viewing = fopen("ticket.dat", "rb");
		FILE* editing = fopen("tempTicHolder.dat", "ab");
		if (viewing == NULL || editing == NULL) {
			printf("Error!!! Unable to open the file\n");
			exit(-1);
		}
		while (fread(&tempTicket, sizeof(Ticket), 1, viewing)) {
			if (tempTicket.sDetail.departureMonth == month && tempTicket.sDetail.departureDay == day) {
				if (tempTicket.tDetail.trainID == trainID) {
					strcpy(tempTicket.ticketStatus, ticStatus);
					fwrite(&tempTicket, sizeof(Ticket), 1, editing);
				}
			}
			else {
				fwrite(&tempTicket, sizeof(Ticket), 1, editing);
			}
		}
		fclose(viewing);
		fclose(editing);

		FILE* clear = fopen("ticket.dat", "wb");
		if (clear == NULL) {
			printf("Error!!! Unable to open the file\n");
			exit(-1);
		}
		fclose(clear);

		FILE* view = fopen("tempTicHolder.dat", "rb");
		FILE* edit = fopen("ticket.dat", "ab");
		if (view == NULL || edit == NULL) {
			printf("Error!!! Unable to open the file\n");
			exit(-1);
		}
		while (fread(&tempTicket, sizeof(Ticket), 1, view)) {
			fwrite(&tempTicket, sizeof(Ticket), 1, edit);
		}
		fclose(view);
		fclose(edit);

		FILE* clear2 = fopen("tempTicHolder.dat", "wb");
		if (clear2 == NULL) {
			printf("Error!!! Unable to open the file\n");
			exit(-1);
		}
		fclose(clear2);

		logDate date;
		sGetCurrentTime(&date.day, &date.month, &date.year, &date.hour, &date.minute);

		FILE* addLog = fopen("staffLog.txt", "a");
		if (addLog == NULL) {
			printf("Unable to open staff.txt\n");
			exit(-1);
		}
		fprintf(addLog, "%s edited all Ticket status for Train ID \"%d\" to \"%s\" for %02d/%02d|%02d-%02d-%d %02d:%02d\n", pStaffInfo->id, trainID, ticStatus, month, day, date.day, date.month, date.year, date.hour, date.minute);

		fclose(addLog);
	}
}
void massDeleteTic(int month, int day, int trainID, Staff* pStaffInfo) {

	Ticket tempTicket;

	FILE* viewing = fopen("ticket.dat", "rb");
	FILE* editing = fopen("tempTicHolder.dat", "ab");
	if (viewing == NULL || editing == NULL) {
		printf("Error!!! Unable to open the file\n");
		exit(-1);
	}

	while (fread(&tempTicket, sizeof(Ticket), 1, viewing)) {
		if (tempTicket.sDetail.departureMonth != month || tempTicket.sDetail.departureDay != day || tempTicket.tDetail.trainID != trainID) {
			fwrite(&tempTicket, sizeof(Ticket), 1, editing);
		}
	}
	fclose(viewing);
	fclose(editing);

	FILE* clear = fopen("ticket.dat", "wb");
	if (clear == NULL) {
		printf("Error!!! Unable to open the file\n");
		exit(-1);
	}
	fclose(clear);

	FILE* view = fopen("tempTicHolder.dat", "rb");
	FILE* edit = fopen("ticket.dat", "ab");
	if (view == NULL || edit == NULL) {
		printf("Error!!! Unable to open the file\n");
		exit(-1);
	}

	while (fread(&tempTicket, sizeof(Ticket), 1, view)) {
		fwrite(&tempTicket, sizeof(Ticket), 1, edit);
	}
	fclose(view);
	fclose(edit);

	FILE* clear2 = fopen("tempTicHolder.dat", "wb");
	if (clear2 == NULL) {
		printf("Error!!! Unable to open the file\n");
		exit(-1);
	}
	fclose(clear2);

	logDate date;
	sGetCurrentTime(&date.day, &date.month, &date.year, &date.hour, &date.minute);

	FILE* addLog = fopen("staffLog.txt", "a");
	if (addLog == NULL) {
		printf("Unable to open staff.txt\n");
		exit(-1);
	}
	fprintf(addLog, "%s deleted all Ticket for Train ID \"%d\" for %02d/%02d|%02d-%02d-%d %02d:%02d\n", pStaffInfo->id, trainID, month, day, date.day, date.month, date.year, date.hour, date.minute);

	fclose(addLog);

}

// ********************************************************************************************************************************
int validateStation(int option) {
	for (int i = 0; i < MAX_STATION; i++) {
		if (option < 1 || option>15) {
			return 1;
		}
	}
	return 0;
}
int availableSeat(int departureStation, int arrivalStation, Train* ptrTrain) {
	int avaiSeat = 0, valid;

	for (int j = 0; j < MAX_COACH; j++) {
		for (int k = 0; k < MAX_SEAT; k++) {
			valid = 0;
			for (int i = departureStation; i < arrivalStation; i++) {
				if (strcmp(ptrTrain->station[departureStation].coach[j].seat[k].status, "Available") != 0) {
					valid = -1; //Detect Same Seat From Departure To Arrival Station
				}
			}
			if (valid == 0) {
				avaiSeat++; //Calculate Available Seat
			}
		}
	}

	return avaiSeat;
}
double calculatePrice(int distance) {
	double price;

	price = ((distance * .2) + 1); //RM0.20 Between Each Station And + RM1

	return price;
}
int vName(int i, char paxName[20]) {

	int length, x;

	length = strlen(paxName);

	for (x = 0; x < length; x++) {
		if (!isalpha(paxName[x]) && paxName[x] != ' ') {
			return 3;
		}
	}

	if (length > 20) {
		return 1;
	}
	else if (length < 3) {
		return 2;
	}
	else
		return 0;
}
int vIC(int i, char paxIC[12]) {

	int length, x;

	length = strlen(paxIC);

	for (x = 0; x < length; x++) {
		if (!isdigit(paxIC[x])) {
			return 2;
		}
	}

	if (length != 12) {
		return 1;
	}
	else
		return 0;
}

void getCurrentDate(int* month, int* day, int m) {

	SYSTEMTIME t; // Declare SYSTEMTIME struct

	GetLocalTime(&t); // Fill out the struct so that it can be used

	*month = t.wMonth;
	*day = t.wDay + m;


	if (*month == 1 || *month == 3 || *month == 5 || *month == 7 || *month == 8 || *month == 10 || *month == 12) {
		if (*day > 31) {
			*month += 1;
			*day -= 30;
		}
	}
	else if (*month == 2) {
		if (t.wYear % 4 == 0) {
			if (*day > 29) {
				*month += 1;
				*day -= 28;
			}

		}
		else if (*day > 28) {
			*month += 1;
			*day -= MAX_STATION;
		}
	}
	else if (*month == 4 || *month == 6 || *month == 9 || *month == 11) {
		if (*day > 30) {
			*month += 1;
			*day -= 29;
		}
	}

	return;
}
int getCurrentTime(int hour, int minute) {

	SYSTEMTIME t; // Declare SYSTEMTIME struct

	GetLocalTime(&t); // Fill out the struct so that it can be used

	hour = t.wHour;
	minute = t.wMinute;

	return 0;
}

void updateValidDate() {

	Train train;

	FILE* scheduleOut = fopen("schedule.dat", "rb");
	FILE* tmp = tmpfile();


	while (fread(&train, sizeof(Train), 1, scheduleOut) != 0) {
		for (int j = 0; j < MAX_STATION; j++) {
			for (int k = 0; k < MAX_COACH; k++) {
				for (int l = 0; l < MAX_SEAT; l++) {
					for (int m = 0; m < 7; m++) {
						getCurrentDate(&train.station[j].coach[k].seat[l].date[m].month, &train.station[j].coach[k].seat[l].date[m].day, m);
					}
				}
			}
		}
		fwrite(&train, sizeof(Train), 1, tmp);
	}

	fclose(scheduleOut);
	rewind(tmp);

	FILE* scheduleIn = fopen("schedule.dat", "wb");

	while (fread(&train, sizeof(Train), 1, tmp) != 0) {
		fwrite(&train, sizeof(Train), 1, scheduleIn);
	}
	fclose(scheduleIn);
	fclose(tmp);

	return;
}

int validateDate(int day, int month, Train* ptrTrain) {
	for (int i = 0; i < 7; i++) {
		if (ptrTrain->station[0].coach[0].seat[0].date[i].day == day && ptrTrain->station[0].coach[0].seat[0].date[i].month == month) {
			return 0;
		}
	}
	return 1;
}
void sStation(Train* ptrTrain) {

	for (int i = 0; i < MAX_STATION; i++) {
		printf("[%-2d]%-22s", (i + 1), ptrTrain->station[i].name);
		if (i % 2 == 1) {
			printf("\n");
		}
	}
}

void previewTicket(Ticket* ptrTicket, struct Member* member) {

	printf("================================================================\n");
	printf("\t\t\t[View Train Ticket]\n");
	printf("================================================================\n");
	printf("  Ticket ID: %-12s \t      Member ID: %-12s\n\n", ptrTicket->ticketID, member->id);
	printf("  From:\n  %s\n\n", ptrTicket->sDetail.departureStation);
	printf("  To:  \n  %s\n", ptrTicket->sDetail.arrivalStation);
	printf("================================================================\n");
	printf("                 \t\t||\n");
	printf("  Pax:           \t\t||    Train:\n");
	printf("  [%d]             \t\t||    %-7d\n", ptrTicket->pax, ptrTicket->tDetail.trainID);
	printf("                 \t\t||\n");
	printf("================================================================\n");
	for (int i = 0; i < ptrTicket->pax; i++) {
		printf("  [Passenger %d]                 ||\n", (i + 1));
		printf("  Passenger Name:               ||    Coach:\n");
		printf("  %-20s      \t||     %c\n", ptrTicket->pDetail[i].paxName, ptrTicket->tDetail.seatDetail[i].coach);
		printf("                 \t\t||\n");
		printf("  Passenger IC:                 ||    Seat:\n");
		printf("  %-12s    \t\t||     %-10.02d\n", ptrTicket->pDetail[i].paxIC, ptrTicket->tDetail.seatDetail[i].seatNumber);
		printf("                 \t\t||\n");
		printf("================================================================\n");
	}

	printf("  Date:      \t  Departure:  \t      Arrival:\n");
	printf("  %02d/%02d    \t  %02d:%02d       \t      %02d:%02d\n\n", ptrTicket->sDetail.departureMonth, ptrTicket->sDetail.departureDay,
		ptrTicket->sDetail.departureHour, ptrTicket->sDetail.departureMinute, ptrTicket->sDetail.arrivalHour, ptrTicket->sDetail.arrivalMinute);
	printf("================================================================\n");
}
void showInvoice(Ticket* ptrTicket) {

	ptrTicket->payment.totalAmount = ptrTicket->payment.amount * ptrTicket->pax;
	printf("\n================================================================\n");
	printf("                           Invoice\n");
	printf("================================================================\n");
	printf("  Ticket ID: %-12s \t       Member ID: %-12s\n\n", ptrTicket->ticketID, ptrTicket->memberID);
	printf("  From:            \t               To:\n");
	printf("  %-20s                 %-20s\n", ptrTicket->sDetail.departureStation, ptrTicket->sDetail.arrivalStation);
	printf("\n  Payment Method: %s\n", ptrTicket->payment.paymentMethod);
	printf("================================================================\n");
	printf("  Item                      Quantity                     Price\n");
	printf("================================================================\n");
	printf("  Train Seat                %d                            $%.2lf\n\n\n", ptrTicket->pax, ptrTicket->payment.amount);
	printf("----------------------------------------------------------------\n");
	printf("  Total Amount                                           $%.2lf\n", ptrTicket->payment.totalAmount);
	printf("----------------------------------------------------------------\n");
}
void viewSummaryTicket(Ticket* ptrTicket) {

	printf("\n=====================================================================================\n\n");
	printf("  Ticket ID:               From:                                 Train ID:                \n");
	printf("  %12s             %-30s        %4d\n", ptrTicket->ticketID, ptrTicket->sDetail.departureStation, ptrTicket->tDetail.trainID);
	printf("\n  Member ID:               To:       \t\t\t         Departure:");
	printf("\n  %-12s             %-30s        %02d:%02d\n\n", ptrTicket->memberID, ptrTicket->sDetail.arrivalStation, ptrTicket->sDetail.departureHour, ptrTicket->sDetail.departureMinute);
	printf("  Booked Date:             Ticket Status:         Date:          Arrival:\n");
	printf("  %02d-%02d-%02d               %-15s        %02d/%02d          %02d:%02d\n", ptrTicket->bDate.bYear, ptrTicket->bDate.bMonth, ptrTicket->bDate.bDay, ptrTicket->ticketStatus,
		ptrTicket->sDetail.departureMonth, ptrTicket->sDetail.departureDay, ptrTicket->sDetail.arrivalHour, ptrTicket->sDetail.arrivalMinute);
	printf("\n=====================================================================================\n");
}
void viewTicket(Ticket* ptrTicket) {

	printf("\n================================================================\n");
	printf("\t\t\t[View Train Ticket]\n");
	printf("================================================================\n");
	printf("  Ticket ID: %-12s \t      Member ID: %-12s\n\n", ptrTicket->ticketID, ptrTicket->memberID);
	printf("  From:\n  %s\n\n", ptrTicket->sDetail.departureStation);
	printf("  To:  \n  %s\n", ptrTicket->sDetail.arrivalStation);
	printf("================================================================\n");
	printf("                 \t\t||\n");
	printf("  Pax:           \t\t||    Train:\n");
	printf("  [%d]             \t\t||    %d\n", ptrTicket->pax, ptrTicket->tDetail.trainID);
	printf("                 \t\t||\n");
	printf("================================================================\n");
	for (int i = 0; i < ptrTicket->pax; i++) {
		printf("  [Passenger %d]                 ||\n", (i + 1));
		printf("  Passenger Name:               ||    Coach:\n");
		printf("  %-20s      \t||     %c\n", ptrTicket->pDetail[i].paxName, ptrTicket->tDetail.seatDetail[i].coach);
		printf("                 \t\t||\n");
		printf("  Passenger IC:                 ||    Seat:\n");
		printf("  %-12s    \t\t||     %-10.02d\n", ptrTicket->pDetail[i].paxIC, ptrTicket->tDetail.seatDetail[i].seatNumber);
		printf("                 \t\t||\n");
		printf("================================================================\n");
	}

	printf("  Date:      \t  Departure:  \t      Arrival:\n");
	printf("  %02d/%02d    \t  %02d:%02d       \t      %02d:%02d\n\n", ptrTicket->sDetail.departureMonth, ptrTicket->sDetail.departureDay,
		ptrTicket->sDetail.departureHour, ptrTicket->sDetail.departureMinute, ptrTicket->sDetail.arrivalHour, ptrTicket->sDetail.arrivalMinute);
	printf("================================================================\n");
	printf("  Item                      Quantity                     Price\n");
	printf("================================================================\n");
	printf("  Train Seat                %d                            $%.2lf\n\n\n", ptrTicket->pax, ptrTicket->payment.amount);
	printf("----------------------------------------------------------------\n");
	printf("  Total Amount                                           $%.2lf\n", ptrTicket->payment.totalAmount);
	printf("================================================================\n");
	printf("  Booked Date:                           Ticket Status: \n");
	printf("  %02d-%02d-%02d                             %s\n", ptrTicket->bDate.bYear, ptrTicket->bDate.bMonth, ptrTicket->bDate.bDay, ptrTicket->ticketStatus);
	printf("================================================================\n");

	printf("\n\n\n");
}

void updateTicket() {

	Ticket tempTicket;

	SYSTEMTIME t;
	GetLocalTime(&t);

	int month, day;

	month = t.wMonth;
	day = t.wDay;

	FILE* viewing = fopen("ticket.dat", "rb");
	FILE* editing = fopen("tempTicHolder.dat", "ab");
	if (viewing == NULL || editing == NULL) {
		printf("Error!!! Unable to open the file\n");
		exit(-1);
	}
	while (fread(&tempTicket, sizeof(Ticket), 1, viewing)) {
		if (tempTicket.sDetail.departureMonth < month || (tempTicket.sDetail.departureMonth == month && tempTicket.sDetail.departureDay < day)) {
			strcpy(tempTicket.ticketStatus, "Invalid");
			fwrite(&tempTicket, sizeof(Ticket), 1, editing);
		}
		else {
			fwrite(&tempTicket, sizeof(Ticket), 1, editing);
		}
	}
	fclose(viewing);
	fclose(editing);

	FILE* clear = fopen("ticket.dat", "wb");
	if (clear == NULL) {
		printf("Error!!! Unable to open the file\n");
		exit(-1);
	}
	fclose(clear);

	FILE* view = fopen("tempTicHolder.dat", "rb");
	FILE* edit = fopen("ticket.dat", "ab");
	if (view == NULL || edit == NULL) {
		printf("Error!!! Unable to open the file\n");
		exit(-1);
	}
	while (fread(&tempTicket, sizeof(Ticket), 1, view)) {
		fwrite(&tempTicket, sizeof(Ticket), 1, edit);
	}
	fclose(view);
	fclose(edit);

	FILE* clear2 = fopen("tempTicHolder.dat", "wb");
	if (clear2 == NULL) {
		printf("Error!!! Unable to open the file\n");
		exit(-1);
	}
	fclose(clear2);
}
void updateTicketLog(char ticID[13], char ticketStatus[20], Staff* pStaffInfo) {
	logDate date;
	sGetCurrentTime(&date.day, &date.month, &date.year, &date.hour, &date.minute);

	FILE* addLog = fopen("staffLog.txt", "a");
	if (addLog == NULL) {
		printf("Unable to open staff.txt\n");
		exit(-1);
	}
	fprintf(addLog, "%s updated Ticket ID \"%s\" status to %s|%02d-%02d-%d %02d:%02d\n", pStaffInfo->id, ticID, ticketStatus, date.day, date.month, date.year, date.hour, date.minute);

	fclose(addLog);
}