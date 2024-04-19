#include <iostream>
#include <sstream>
#include <fstream>

#define TRAIN 1
#define SUBWAY 2

#define TRAINKID 20000
#define TRAINADULT 23700
#define SUBWAYKID 450
#define SUBWAYADULT 1250

using namespace std;

enum MENU{
    CREATE=1,
    SHOWLIST,
    SEARCH,
    PRINTFILE,
    EXIT
};

struct trainPath{ //기차역 정보. 
    string station;
};
struct subwayPath{ //지하철역 정보. 
    int transOp; // 5010 값이면 환승OK. 
    string station;
};

class Ticket{
protected:
    int type;
    int ticketNumber; 

    int kidNum, adultNum, totalNum; // 아이수, 어른수, 총인원수 
    int kidPrice, adultPrice, totalPrice; //아이가격, 어른가격, 총가격 

    string src, dst; //출발역 도착역 

public:
    Ticket(int type,int ticketNumber, int kidNum, int adultNum, string src, string dst, int kid, int adult) {
        this->type = type;
        this->ticketNumber = ticketNumber;
        this->kidNum = kidNum;
        this->adultNum = adultNum;
        this->src = src;
        this->dst = dst;
        this->kidPrice = kid;
        this->adultPrice = adult;

        this->totalNum = kidNum + adultNum;
        this->totalPrice = kidNum*kid + adultNum*adult; //금액계산
    }
    bool isEqualTicNumber(int n) { //티켓번호 비교 
        if(this->ticketNumber == n)
            return true;
        else
        return false;
    }
    string intToString(int n) {
        stringstream s;
        s << n;

        return s.str();
    }

    virtual void showInfo() = 0;
    virtual void filePrint() = 0;
};

class Train : public Ticket {
private:
    int suiteOp; //특실 
    int hour, min; //시간, 분 
public:
    Train(int type,int ticketNumber ,int kidNum, int adultNum, string src, string dst, int hour, int min, int suiteOp)
    	: Ticket(type,ticketNumber, kidNum, adultNum, src, dst, TRAINKID, TRAINADULT) {
        this->suiteOp = suiteOp;
        this->hour = hour;
        this->min = min;
    }
    virtual void showInfo() {
    	cout<<" "<<this->ticketNumber<<")"<<" 특실옵션 : ["<<this->suiteOp<<"] 출발역 : ["<<this->src<<"] 도착역 : ["<<this->dst<<"] 총 가격 : ["<<this->totalPrice<<"]"<<endl;
        return;
    }
    virtual void filePrint() { //train.txt에서 필요정보 뽑아서 출력 
        ofstream fout; 

    	string filenameHead = "train["+intToString(this->ticketNumber);
        string filenameTail = "].txt";
        string filename = filenameHead+filenameTail;

        fout.open(filename.c_str());

        fout<<"기차 티켓 출력 서비스입니다\n";
        fout<<"-----------------------------------------\n";
        fout<<"기차 통합 티켓표"<<endl;
        fout<<"티켓 번호 : ["<<this->ticketNumber<<"]"<<endl;
        fout<<"탑승 인원 : ["<<this->totalNum<<"]"<<endl;
        fout<<"출발역 : ["<<this->src<<"]"<<endl;
    	fout<<"도착역 : ["<<this->dst<<"]"<<endl;
        fout<<"출발시간 : ["<<this->hour<<":"<<this->min<<"]"<<endl;
        fout<<"금액 : ["<<this->totalPrice<<"]"<<endl;
        fout<<"-----------------------------------------\n";

        return;
    }
};

class Subway : public Ticket {
private:
    int trans; // 환승횟수 
public:
    Subway(int type, int ticketNumber, int kidNum, int adultNum, string src, string dst, struct subwayPath *list)
    	: Ticket(type, ticketNumber, kidNum, adultNum, src, dst, SUBWAYKID, SUBWAYADULT) {
    this->trans = 0;
    bool flag = false;

    for(int i=0;true;i++) {
        if(list[i].station == src)
            flag = true;
    	if(list[i].transOp == 5010 && flag ==true)
            this->trans++;
        if(list[i].station == dst) {
            if(list[i].transOp == 5010 && flag == true)
            	this->trans++;
            break;
        }	
    }
    	return;
    }
    virtual void showInfo() {
        cout<<" "<<this->ticketNumber<<")"<<" 환승역 개수 : ["<<this->trans<<"] 출발역 : ["<<this->src<<"] 도착역 : ["<<this->dst<<"] 총 가격 : ["<<this->totalPrice<<"]\n"<<endl;
        	return;
    }

    virtual void filePrint() {  
        ofstream fout;

    	string filenameHead = "subway["+intToString(this->ticketNumber);
        string filenameTail = "].txt";
        string filename = filenameHead+filenameTail;

        fout.open(filename.c_str());

    	fout<<"지하철 티켓 출력 서비스입니다\n";

    	for(int i=0,kid=this->kidNum,adult=this->adultNum;i<this->totalNum;i++) { //subway.txt에서 필요정보 뽑아서 출력 
            if(i==0)
            	fout<<"-----------------------------------------";
            if(kid!=0) {
                fout<<"어린이용 티켓표"<<endl;
                fout<<"티켓 번호 : ["<<this->ticketNumber<<"]"<<endl;
                fout<<"출발역 : ["<<this->src<<"]"<<endl;
                fout<<"도착역 : ["<<this->dst<<"]"<<endl;
            	fout<<"금액 : ["<<SUBWAYKID<<"]"<<endl;
                fout<<"-----------------------------------------\n";
                kid--;
            }
            if(adult!=0) {
                fout<<"성인용 티켓표"<<endl;
                fout<<"티켓 번호 : ["<<this->ticketNumber<<"]"<<endl;
                fout<<"출발역 : ["<<this->src<<"]"<<endl;
                fout<<"도착역 : ["<<this->dst<<"]"<<endl;
                fout<<"금액 : ["<<SUBWAYADULT<<"]"<<endl;
                fout<<"-----------------------------------------\n";
                adult--;
            }

        }

        return;
	}
};

class Manager { //티켓관리클래스 
private:
    Ticket *tic[30]; //티켓껍데기를 30개 만듦 
    int count;

    struct trainPath tList[10];
    struct subwayPath sList[50];
    int tCount;
    int sCount;

public:
    Manager() {
    	this->count = 0;
        this->tCount = 0;
        this->sCount = 0;
    }
	~Manager() {
		for(int i=0;i<this->count;i++)
			delete tic[i];
		}
        void readTrainPathList() { //기차노선정보를 train.txt에서 가져옴 
            ifstream fin;

            fin.open("./resource/train.txt");
            if(fin.fail()) {
        		cout<<"train.txt가 없습니다."<<endl;
            	return;
            }

            while(!fin.eof()) {
            	fin>>tList[tCount++].station;
            }


            return;
    	}
        void readSubwayPathList() {  //지하철노선정보를 subway.txt에서 가져옴
            ifstream fin;

            fin.open("./resource/subway.txt");
                if(fin.fail()) {
                	cout<<"subway.txt가 없습니다."<<endl;
                    return;
                }

            while(!fin.eof()) {
                fin>>sList[sCount++].station;


                if(sList[sCount-1].station.substr(0,1) == "*") {
                    sList[sCount-1].transOp = 5010;
                    sList[sCount-1].station = sList[sCount-1].station.substr(1);
                }
            }

            return;
        }
    	int printMenu() {
            int c;

            cout << endl;
			cout << "+-------------------------+" << endl;
			cout <<    ">> 기차표 예매 프로그램 <<" << endl;
			cout << "+-------------------------+" << endl;
			cout << endl; 
			cout << "기  차 : 서울역 - 대전역 -  동대구역 - 부산역까지 운행하는 KTX 열차 " << endl;
			cout << "지하철 : 당고개 ~ 오이도까지 운행하는 4호선 열차" << endl;
			cout << endl; 
			cout << "기차표 예매  프로그램을 시작합니다..." << endl;
			cout << "1. 티켓 예매 하기 " << endl;
			cout << "2. 예약 전체 리스트 출력하기 " <<endl; 
			cout << "3. 예약번호로 검색하기  "<< endl; 
			cout << "4. 티켓을 파일에 출력하기"<<endl;
        	cout << "5. 종료 "<<endl;
            cout << " 선택 > ";
            cin  >> c;

            return c;
        }
        void createTicket() {
            int type;
        	int kidNum, adultNum;
            string src, dst;
            int suiteOp; // 특실
        	int hour, min;

            bool srcFlag=false, dstFlag=false;

            cout<<" 어떤 열차를 예매하시겠습니까? ( 1. 기차 / 2. 지하철 ) >> ";
            cin>>type;

            if(type == TRAIN) {
                cout<<"아이수 입력 >> "; cin>>kidNum;
                cout<<"어른수 입력 >> "; cin>>adultNum;
            	cout<<"출발지를 입력하세요 >> "; cin>>src;
                cout<<"도착지를 입력하세요 >> "; cin>>dst;

            for(int i=0;i<this->tCount;i++) {
                if(tList[i].station == src)
                    srcFlag=true;
            	if(tList[i].station == dst)
            	dstFlag=true;
            }

            if(srcFlag == false || dstFlag == false) {
                if(srcFlag == false)
                    cout<<" 없는 출발역입니다. "<<endl;
                if(dstFlag == false)
                    cout<<" 없는 도착역입니다. "<<endl;
                return;
            }

            cout<<"출발시간을 입력하세요 >> "; cin>>hour;
            cout<<"출발분을 입력하세요 >> "; cin>>min;

            cout<<"특실 여부를 입력하세요( yes - 0 / no - 1) >> "; cin>>suiteOp;
            tic[this->count++] = new Train(type,this->count,kidNum,adultNum,src,dst,hour,min,suiteOp);

            }
    		else if(type == SUBWAY) {
	            cout<<"아이수 입력 >> "; cin>>kidNum;
	            cout<<"어른수 입력 >> "; cin>>adultNum;
	            cout<<"출발지를 입력하세요 >> "; cin>>src;
	            cout<<"도착지를 입력하세요 >> "; cin>>dst;
	
	            for(int i=0;i<this->sCount;i++) {
                    if(sList[i].station == src)
                        srcFlag=true;
                    if(sList[i].station == dst)
                    	dstFlag=true;
	            }
	            if(srcFlag == false || dstFlag == false) {
	                if(srcFlag == false)
	                    cout<<" 없는 출발역입니다. "<<endl;
	                if(dstFlag == false)
	                    cout<<" 없는 도착역입니다. "<<endl;
	                    return;
                }
                tic[this->count++] = new Subway(type,this->count,kidNum,adultNum,src,dst,this->sList);
            }
            else {
                cout<<"잘못된 입력입니다"<<endl;
                return;
            }
            	return;
        }

        void showListByAll() {
            for(int i=0;i<this->count;i++)
            	tic[i]->showInfo();
            return;
        }

        void searchTicketByNumber() {
            int number;

            cout<<"티켓 번호를 입력하세요 >> ";
            cin>>number;

            for(int i=0;i<this->count;i++) {
                if(tic[i]->isEqualTicNumber(number) == true) {
                    tic[i]->showInfo();
                    break;
                }
            }
            return;
        }
        void filePrint() {
	        for(int i=0;i<this->count;i++) {
	                tic[i]->filePrint();
	        }
	        return;
        }
};

int main(void) 
{
    Manager *man = new Manager();

    man->readTrainPathList();
    man->readSubwayPathList();


    while(1) {
        switch(man->printMenu())
        {
            case CREATE:
                man->createTicket();
                break;
            case SHOWLIST:
                man->showListByAll();
                break;
            case SEARCH:
                man->searchTicketByNumber();
                break;
            case PRINTFILE:
                man->filePrint();
                break;
            case EXIT:
                goto EXITLABEL;
                break;
	    }
	}
	EXITLABEL:
	delete man;
	
    return 0;
}
