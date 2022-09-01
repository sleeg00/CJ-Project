#include <iostream>
#include <fstream>
#include <string>
#include<vector>
#include <sstream>
#include<math.h>
#include<queue>
#include<algorithm>
#define radius(x) x*3.141592/180.0
using namespace std;
struct Point {
	double x;
	double y;
	Point() {};
	Point(double x, double y) {
		this->x = x;
		this->y = y;
	}
};
struct Garage {
	string name;
	Point p;
	Garage(string name, Point p) {
		this->name = name;
		this->p = p;
	}
};
struct P_dis {
	Point p;
	double dis;
	int index;
	int qty;
	int condition;
	int s_time;
	int start_time;
	int end_time;
	string essential;
	P_dis(Point p, double dis, int index, int qty, int condition, int s_time, int start_time, int end_time) {
		this->p = p;
		this->dis = dis;
		this->index = index;
		this->qty = qty;
		this->condition = condition;
		this->s_time = s_time;
		this->start_time = start_time;
		this->end_time = end_time;
	}
	P_dis(Point p, double dis,int index,int qty, int condition, int s_time, int start_time, int end_time,string essential) {
		this->p = p;
		this->dis = dis;
		this->index = index;
		this->qty = qty;
		this->condition = condition;
		this->s_time = s_time;
		this->start_time = start_time;
		this->end_time = end_time;
		this->essential = essential;
	}
	bool operator < (const P_dis& a)const {
		if (essential.length() != a.essential.length()) // 주문의 필수 배송차량 우선순위
			return essential.length() < a.essential.length();
		else {
			//if (start_time != a.start_time) // 주문의 배송 시작시간 우선순위
			//	return start_time > a.start_time;
			//else
				return dis > a.dis; // 주문의 배달 거리 우선순위
		}
			
	}

};
struct info {
	string d_id;
	string number_id;
	int index;
	string type;
	double x;
	double y;
	string end_time;
	string wait_time;
	string depart_time;
	double km;
	string m_time;
	info(string d_id, string number_id, int index, string type, double x, double y, string end_time, string wait_time, string depart_time, double km, string m_time) {
		this->d_id = d_id;
		this->number_id = number_id;
		this->index = index;
		this->type = type;
		this->x = x;
		this->y = y;
		this->end_time = end_time;
		this->wait_time = wait_time;
		this->depart_time = depart_time;
		this->km = km;
		this->m_time = m_time;
	}
	
	string data_out() {
		string str = "";
		string sx = to_string(x);
		string sy = to_string(y);
		string skm = to_string(km);

		sx = sx.substr(0, sx.size() - 4);
		sy = sy.substr(0, sy.size() - 4);
		skm = skm.substr(0, skm.size() - 4);
		str = d_id + "	" + number_id + "	" + to_string(index)+"	"+type + "	" + to_string(x).substr(0, to_string(x).length() - 3) +"	" + to_string(y).substr(0, to_string(y).length() - 3) + "	" + end_time + "	" + wait_time + "	" + depart_time + "	" + to_string(km).substr(0, to_string(km).length() - 4) + "	" + m_time + "\n";
		return str;
	}
	bool operator<(const info& a)const {

		return d_id.compare(a.d_id);
	}
};
struct sim_result {
	string d_id;
	int sum;
	string start_time;
	string end_time;
	int p_sum;
	string total_time;
	string move_time;
	string wait_time;
	double m_km;
	sim_result() {
	}
	
	sim_result(string d_id,	int sum, string start_time,	string end_time,int p_sum,string total_time,string move_time,string wait_time,	double m_km) {
		this->d_id = d_id;
		this->sum = sum;
		this->start_time = start_time;
		this->end_time = end_time;
		this->p_sum = p_sum;
		this->total_time = total_time;
		this->move_time = move_time;
		this->wait_time = wait_time;
		this->m_km = m_km;
	}
	string data_out() {
		string str = "";
		string skm = to_string(m_km);

		skm = skm.substr(0, skm.size() - 4);
		str = d_id + "	" + to_string(sum) + "	" + start_time + "	" + end_time + "	" + to_string(p_sum) + "	" + total_time + "	" + move_time + "	" + wait_time + "	" + skm + "\n";
		return str;
	}

};
vector<Garage> ge;
vector<vector<string>> driver;
vector<vector<string>> order;
priority_queue<P_dis> pq;
int ch[2199];
void setup() {
	ge.push_back(Garage("Depot1", Point(37.562, 126.973)));
	ge.push_back(Garage("Depot2", Point(37.775, 126.715)));
	ge.push_back(Garage("Depot3", Point(37.421, 126.910)));
	string str_buf;
	fstream fs;
	fs.open("data.csv", ios::in);

	vector<string>tmp;
	int cnt = 0;
	while (!fs.eof()) {
		getline(fs, str_buf);
		istringstream line(str_buf);
		while (getline(line, str_buf, ',')) {
			tmp.push_back(str_buf);
		}

		driver.push_back(tmp);
		tmp.clear();
	}
	fs.close();
	fs.open("order.csv", ios::in);
	while (!fs.eof()) {
		getline(fs, str_buf);
		istringstream line(str_buf);
		while (getline(line, str_buf, ',')) {
			tmp.push_back(str_buf);
		}

		order.push_back(tmp);
		tmp.clear();
	}
	fs.close();

	driver.pop_back();
	order.pop_back();

}
double getDistance(Point p1, Point p2)
{
	double radius = 6371.009;

	double dLat = radius((p2.x - p1.x));
	double dLon = radius((p2.y - p1.y));


	double dt = dLat / 2;
	double dn = dLon / 2;
	p1.x = radius(p1.x);
	p2.x = radius(p2.x);

	double a = sin(dt) * sin(dt) + sin(dn) * sin(dn) * cos(p1.x) * cos(p2.x);
	double c = 2 * atan2(sqrt(a), sqrt(1 - a));
	double dDistance = radius * c * 1.4;

	return  dDistance;
}
string getSTime(int sec) {
	int hour;
	int minute;
	int s_sec;
	
	minute = sec / 60;
	s_sec = sec % 60;

	hour = minute / 60;
	minute = minute % 60;
	
	string h = to_string(hour);
	string m = to_string(minute);
	string s = to_string(s_sec);
	while (h.length() < 2) {
		h = "0" + h;
	}
	while (m.length() < 2) {
		m = "0" + m;
	}
	while (s.length() < 2) {
		s = "0" + s;
	}
	

	string str = h +":" + m + ":" + s;
	return str;
}

int getSec(double km) {
	int s;
	s = km / 40 * 3600;

	return s;
}
bool cmp(vector<info>& v1, vector<info>& v2) {
	int a = stoi(v1[0].d_id.substr(6));
	int b= stoi(v2[0].d_id.substr(6));
	return a < b;
	//return v1[0].d_id.compare(v2[0].d_id);
}
bool cmp2(sim_result& v1, sim_result& v2) {
	int a = stoi(v1.d_id.substr(6));
	int b = stoi(v2.d_id.substr(6));
	return a < b;
	//return v1[0].d_id.compare(v2[0].d_id);
}
int main() {
	clock_t start, finish;
	double duration;

	start = clock();
	ios_base::sync_with_stdio(false); cin.tie(NULL);
	setup();
	
	Point current;
	Point end;
	vector<vector<info>> v(37);
	vector<sim_result> re(37);
	
	for (int i = 0; i < driver.size(); i++) {
		
		if (driver[i][6] == "Depot1")
			current = ge[0].p;
		else if (driver[i][6] == "Depot2")
			current=ge[1].p;
		else
			current = ge[2].p;

		if (driver[i][7] == "Depot1")
			end = ge[0].p;
		else if (driver[i][7] == "Depot2")
			end = ge[1].p;
		else
			end = ge[2].p;

		
		int st = (stoi(driver[i][2].substr(11, 1))) * 3600 +1;
		int kk = st;
		int wt = 0;
		int d_con = driver[i][4].length();
		int et = (stoi(driver[i][3].substr(11, 1))) * 3600;
		v[i].push_back(info(driver[i][0], driver[i][6], 0,"Depot", current.x, current.y, "-", "00:00:00", "2022-06-20 오전 "+ getSTime(st), 0, "00:00:00"));
		for (int j = 0; j < order.size(); j++) {
			if (ch[j] == 1)continue;
			Point tmp = Point(stod(order[j][3]), stod(order[j][4]));
			int qty = stoi(order[j][2]);
			int condition = order[j][6].length();
			int s_time = stoi(order[j][7]);
			long start_time = (stoi(order[j][8].substr(11, 1))) * 3600;
			long end_time = (stoi(order[j][9].substr(11, 1))) * 3600;
			if (condition < 10)
			{
				if (d_con != condition)
					continue;
			}
			
			if (getSec(getDistance(tmp, current))+st >= start_time&& getSec(getDistance(tmp, current)) + st+(s_time*60)<end_time) {
				
				if (order[j].size() > 10) {
					string essential = order[j][10];
					if (essential == driver[i][0])
						pq.push(P_dis(tmp, getDistance(current, tmp), j, qty, condition, s_time, start_time, end_time, essential));

				}
				else {
					
						pq.push(P_dis(tmp, getDistance(current, tmp), j, qty, condition, s_time, start_time, end_time));
							
				}
			}			
			
		}

		int count = 1;
		double dis = 0;
		
		while (!pq.empty()) {
			
			Point next = pq.top().p;
			int index = pq.top().index;
			int lst = pq.top().start_time;
			int let = pq.top().end_time;
			int s_time = pq.top().s_time;
			pq.pop();
			
			if (dis + getDistance(current, next) + getDistance(next, end) < 110.0&&getSec(getDistance(current,next))+st+(s_time*60)+getSec(getDistance(next,end))<et) {
				dis += getDistance(current, next);
				st += getSec(getDistance(next, current));
				v[i].push_back(info(driver[i][0], order[index][0], count,"배송지", next.x, next.y, "2022-06-20 오전 " + getSTime(st), "00:00:00", "2022-06-20 오전 " + getSTime(st+(s_time*60)), getDistance(next, current), getSTime(getSec(getDistance(current, next)))));
				count++;
				st += s_time * 60;
				current = next;
				
				ch[index] = 1;
				pq = priority_queue<P_dis>();
				for (int k = 0; k < order.size(); k++) {
					if (ch[k] == 1)continue;
					Point tmp = Point(stod(order[k][3]), stod(order[k][4]));
					int qty = stoi(order[k][2]);
					int condition = order[k][6].length();
					int s_time = stoi(order[k][7]);
					long start_time = (stoi(order[k][8].substr(11, 1))) * 3600;
					long end_time = (stoi(order[k][9].substr(11, 1))) * 3600;
					if (condition < 10)
					{
						if (d_con != condition)
							continue;
					}
					if (getSec(getDistance(tmp, current)) + st >= start_time && getSec(getDistance(tmp, current)) + st + (s_time * 60) < end_time) {

						if (order[k].size() > 10) {
							string essential = order[k][10];
							if(essential==driver[i][0])
								pq.push(P_dis(tmp, getDistance(current, tmp), k, qty, condition, s_time, start_time, end_time, essential));
						
						}
						else {
							
							pq.push(P_dis(tmp, getDistance(current, tmp), k, qty, condition, s_time, start_time, end_time));
							

						}
					}
				}
			}
			else {
				
				continue;

			}

			if (count == 51) {
				pq = priority_queue<P_dis>();
				break;
			}

		}
		v[i].push_back(info(driver[i][0], driver[i][7], count,"Depot", end.x, end.y, "2022-06-20 오전 "+ getSTime(st+ getSec(getDistance(current, end))), "0:00:00", "-", getDistance(end,current), getSTime(getSec(getDistance(current,end)))));
		dis += getDistance(current, end);
		int total_t = (st + getSec(getDistance(current, end))) - kk;
		
		
		re[i]=(sim_result(driver[i][0], count - 1, "2022-06-20 오전 " + getSTime(kk), "2022-06-20 오전 " + getSTime(st + getSec(getDistance(current, end))), count - 1, getSTime(total_t), getSTime(getSec(dis)), "0:00:00", dis));
	}
	sort(v.begin(), v.end(),cmp);
	sort(re.begin(), re.end(), cmp2);
	for (int i = 0; i < 37; i++) {
			
			cout<<v[i][0].d_id<<" : "<<v[i].size()<<endl;
	}
	
	ofstream file("result.txt");
	if (file.is_open()) {
		for (int i = 0; i < 37; i++) {
			
			for (int j = 0; j < v[i].size(); j++) {
				file << v[i][j].data_out();
			}

		}
		file.close();
	}
	else
	{
		cout << "error";
	}
	file.open("simple.txt", ios::out);
	if (file.is_open()) {
		for (int i = 0; i < 37; i++) {
			sim_result sr = re[i];
			file << re[i].data_out();

		}
		file.close();
	}
	else
	{
		cout << "error";
	}
	file.open("no_st.txt", ios::out);
	if (file.is_open()) {
		for (int i = 0; i < order.size(); i++) {
			if (ch[i] == 0) {
				for (int j = 0; j < order[i].size(); j++) {
					file << order[i][j] << "	";
				}
				file << "\n";
			}
			

		}
		file.close();
	}
	else
	{
		cout << "error";
	}
	finish = clock();

	duration = (double)(finish - start) / CLOCKS_PER_SEC;
	printf("%f초\n", duration);
}