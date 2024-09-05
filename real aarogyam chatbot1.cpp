#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <queue>
#include <limits>

using namespace std;

// struct to represent a symptom
struct Symptom 
{
    int id;
    string name;
};

const int n = 5; // number of time slots
int ct=0;// number of patients

struct Appointment 
{
    int patient_id;
    string doctor_name;
    string patient_name;
    int age;
    string disease;
    int slot;
};

// struct to represent an edge in the graph
struct Edge 
{
    int dest;
    int weight;
};

// struct to represent a node in the graph
struct Node 
{
    int id;
    string name;
    vector<Edge> edges;
};

// struct to represent a condition
struct Condition 
{
    int id;
    string name;
    vector<int> symptom_ids; // ids of the symptoms associated with this condition
    int risk_score; // 1-10 scale, with 10 being most serious
};

struct Hospital 
{
    string name;            // Hospital name
    //string area;            // Area of the hospital
    map<string, int> dist;  // Distances to other hospitals
};

class AppointmentBooking
{
    vector<int> adj[2*n+2];
    int cap[2*n+2][2*n+2];
    bool vis[2*n+2];
    
    public:
        AppointmentBooking()
        {
            for (int i = 1; i <= n; i++) 
            {
                add_edge(0, i, 1); // source to time slot
                add_edge(i+n, 2*n+1, 1); // time slot to sink
                add_edge(i, i+n, 1); // time slot to its corresponding node
            }
        }
        
        void add_edge(int u, int v, int c) 
        {
            adj[u].push_back(v);
            adj[v].push_back(u);
            cap[u][v] = c;
        }
        
        bool dfs(int u, int t) 
        {
            if (u == t)
            { 
                return true;
            }
        
            vis[u] = true;
            for (int v : adj[u]) 
            {
                if (!vis[v] && cap[u][v] > 0 && dfs(v, t)) 
                {
                    cap[u][v]--;
                    cap[v][u]++;
                    return true;
                }
            }
            return false;
        }
        
        bool book_appointment(Appointment appt) 
        {
            int u = appt.slot;
            int v = n + appt.slot;
            if (cap[u][v] > 0) 
            {
                cap[0][u]--;
                cap[u][v]--;
                cap[v][2*n+1]--;
                return true;
            }
            return false;
        }
        
        void timeSlots(string doctor_name,string disease,vector<Appointment> &BookedPatientsData)
        {
            string patient_name;
            int age;
            cout<<"According to my knowledge,Dr."<<doctor_name<<" has speciality in treating "<<disease<<endl;
            
            for (int i = 0; i < 3; i++) 
            {
                int slot;
                cout << "\nHe is available for online consultation in the following time slots:" << endl;
                cout << "Slot 1: 9:00 AM to 10:00 AM" << endl;
                cout << "Slot 2: 10:00 AM to 11:00 AM" << endl;
                cout << "Slot 3: 11:00 AM to 12:00 PM" << endl;
                cout << "Slot 4: 12:00 PM to 1:00 PM" << endl;
                cout << "Slot 5: 1:00 PM to 2:00 PM" << endl;
                cout<<"\nCould you please enter you name?"<<endl;
                cin.ignore();
                getline(cin,patient_name);
                
                cout<<"\nCould you enter your age,please?"<<endl;
                cin>>age;
                
                cout << "\nEnter the desired time slot (enter the slot number): ";
                cin >> slot;
                
                if (slot < 1 || slot > n) 
                {
                    cout << "Invalid slot number. Please choose again." << endl;
                } 
                else 
                {
                    ct++;
                    Appointment appt = {ct,doctor_name,patient_name, age,disease,slot};
                    BookedPatientsData.push_back(appt);
                    if (book_appointment(appt)) 
                    {
                        cout << "Appointment for " << patient_name << " has been booked for slot " << slot << "  successfully." << endl;
                        break;
                    } 
                    else 
                    {
                        cout << "Sorry,the appointment for " <<patient_name<< " could not be booked for slot as it is already booked ny someone. " << slot << ". Please choose another time slot." << endl;
                        
                    }
                }
            
            }
        }
};
// Graph data structure to store hospital distances
class Graph 
{
    public:
        // Map to store hospital distances for each area
        map<string, map<string, int>> distances;
    
        // Add edge to the graph
        void add_edge(string u, string v, int w) 
        {
            distances[u][v] = w;
            distances[v][u] = w;
        }
};

// Dijkstra's algorithm to find the shortest hospital distance
void dijkstra(Graph graph, string start) 
{
    // Set of visited vertices
    map<string, bool> visited;

    // Distance map to keep track of shortest distances
    map<string, int> distance;

    // Priority queue to store vertices in non-decreasing order of distance
    priority_queue<pair<int, string>, 
    vector<pair<int, string>>, greater<pair<int, string>>> pq;

    // Initialize distances and visited vertices
    for (auto const& x : graph.distances) 
    {
        distance[x.first] = numeric_limits<int>::max();
        visited[x.first] = false;
    }

    // Set the distance of the start vertex to zero and add it to the queue
    distance[start] = 0;
    pq.push(make_pair(0, start));

    // Dijkstra's algorithm
    while (!pq.empty()) 
    {
        // Get the vertex with the smallest distance
        string u = pq.top().second;
        pq.pop();
        
        // If the vertex has not been visited, mark it as visited
        if (!visited[u]) 
        {
            visited[u] = true;
            // Update the distances of its adjacent vertices
            for (auto const& v : graph.distances[u]) 
            {
                int weight = v.second;
                string vertex = v.first;
                if (!visited[vertex]) 
                {
                    if (distance[u] + weight < distance[vertex]) 
                    {
                        distance[vertex] = distance[u] + weight;
                        pq.push(make_pair(distance[vertex], vertex));
                    }
                }
            }
        }
    }
    int minDist = numeric_limits<int>::max();
    string shortestHospital;
    for (const auto& d : distance) 
    {
        string ss=d.first;
        if (d.second < minDist && ss.find("Hospital") != string::npos) 
        {
            minDist = d.second;
            shortestHospital = d.first;
            //cout<< shortestHospital<<endl;
        }
    }
    if(shortestHospital.empty()){
           cout<<"\nI could not suggest you a hospital which is near your locaity"<<endl;
       }
        else{
             cout<< "\nThe hospital which is located near "<<start<<" is "<<shortestHospital<<" at a distance of "<<distance[shortestHospital]<<" KM"<<endl;
         
        }
   // cout<<"\nThank You for contacting AAROKIYAM CHATBOT\n\n"<<endl;
    // Return the shortest distance to the end vertex
    //return distance[end];
}
// function to initialize the symptoms and conditions
void initialize(vector<Symptom>& symptoms, vector<Condition>& conditions) 
{
    symptoms.push_back({1, "Fever"});
    symptoms.push_back({2, "Cough"});
    symptoms.push_back({3, "Shortness of breath"});
    symptoms.push_back({4, "Chest pain"});
    symptoms.push_back({5, "Headache"});
    symptoms.push_back({6, "Nausea"});
    symptoms.push_back({7, "Dizziness"});
    symptoms.push_back({8, "skin rash"});
    symptoms.push_back({9, "joint pain"});
    symptoms.push_back({10, "vomiting"});
    symptoms.push_back({11, "fatigue"});
    symptoms.push_back({12, "weight loss"});
    symptoms.push_back({13, "burning micturition"});
    symptoms.push_back({14, "itching"});
    symptoms.push_back({15, "stomach pain"});
    symptoms.push_back({16, "restlessness"});

    conditions.push_back({1, "Common cold", {1, 2}, 2});
    conditions.push_back({2, "Flu", {1, 2, 3}, 4});
    conditions.push_back({3, "Pneumonia", {1, 2, 3, 4}, 8});
    conditions.push_back({4, "Migraine", {5}, 2});
    conditions.push_back({5, "Food poisoning", {6, 7}, 5});
    conditions.push_back({6, "Dengue", {5,9,10,11}, 7});
    conditions.push_back({7, "Drug reaction", {13, 14,15}, 6});
    conditions.push_back({8, "Diabetes", {11, 12,16}, 8});
   
}

// function to get the index of the symptom with the given id
int findSymptomIndex(const vector<Symptom>& symptoms, int id) 
{
    for (int i = 0; i < symptoms.size(); i++) 
    {
        if (symptoms[i].id == id) 
        {
            return i;
        }
    }
    return -1;
}

// function to get the index of the condition with the given id
int findConditionIndex(const vector<Condition>& conditions, int id) 
{
    for (int i = 0; i < conditions.size(); i++) 
    {
        if (conditions[i].id == id) 
        {
            return i;
        }
    }
    return -1;
}

// function to build the graph based on the symptoms and conditions
void buildGraph(const vector<Symptom>& symptoms, const vector<Condition>& conditions, vector<Node>& graph) 
{
    // create a node for each symptom
    for (auto symptom : symptoms) 
    {
        graph.push_back({symptom.id, symptom.name});
    }

    // connect symptoms that are commonly associated with each other
    for (auto condition : conditions) 
    {
        for (int i = 0; i < condition.symptom_ids.size(); i++) 
        {
            for (int j = i+1; j < condition.symptom_ids.size(); j++) 
            {
                int src = findSymptomIndex(symptoms, condition.symptom_ids[i]);
                int dest = findSymptomIndex(symptoms, condition.symptom_ids[j]);
                int weight = condition.risk_score;
                graph[src].edges.push_back({dest, weight});
                graph[dest].edges.push_back({src, weight});
            }
        }
    }
}

// function to find the most likely
// condition given a set of symptoms
int findMostLikelyCondition(const vector<Node>& graph, const vector<Symptom>& symptoms, const vector<Condition>& conditions, const vector<int>& symptom_ids) 
{
    int n = graph.size();
    // initialize the distance array to infinity for all nodes except the first
    vector<int> dist(n, numeric_limits<int>::max());
    int src = findSymptomIndex(symptoms, symptom_ids[0]);
    dist[src] = 0;
    
    // use Dijkstra's algorithm to find the shortest path from the source to all other nodes
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({0, src});
    while (!pq.empty()) 
    {
        int u = pq.top().second;
        pq.pop();
        for (auto edge : graph[u].edges) 
        {
            int v = edge.dest;
            int weight = edge.weight;
            if (dist[v] > dist[u] + weight) 
            {
                dist[v] = dist[u] + weight;
                pq.push({dist[v], v});
            }
        }
    }

    // find the condition with the highest risk score among those that have all the symptoms
    int max_risk_score = 0;
    int max_risk_score_index = -1;
    for (int i = 0; i < conditions.size(); i++) 
    {
        bool has_all_symptoms = true;
        for (auto symptom_id : conditions[i].symptom_ids) 
        {
            if (find(symptom_ids.begin(), symptom_ids.end(), symptom_id) == symptom_ids.end()) 
            {
                has_all_symptoms = false;
                break;
            }
        }
        if (has_all_symptoms && conditions[i].risk_score > max_risk_score) 
        {
            max_risk_score = conditions[i].risk_score;
            max_risk_score_index = i;
        }
    }
    return max_risk_score_index;
}


int main() 
{
    vector<Symptom> symptoms;
    vector<Condition> conditions;
    initialize(symptoms, conditions);
    vector<Node> graph;
    buildGraph(symptoms, conditions, graph);
    Graph hospital;
    string inputarea;
    AppointmentBooking Ram,Raj,Sathya;
    vector<Appointment> BookedPatientsData;
    // Add edges to the graph
     string ch1;
    vector<Hospital> hospitals =
    {
        {"Apollo Hospital", 
            {
                { "Greams Road",0},
                {"Adyar", 15},
                {"Alwarpet", 12},
                {"Anna Nagar", 18},
                {"Besant Nagar", 10},
                {"Chrompet", 20},
                {"Egmore", 10},
                {"Guindy", 29},
                {"Kilpauk", 15},
                {"Medavakkam", 18},
                {"Mylapore", 8},
                {"Nanganallur", 25},
                {"Perambur", 20},
                {"T Nagar", 10},
                {"Tambaram", 25},
                {"Velachery", 12}
            }
            
        },
        {"Government General Hospital", 
            {
                {"Apollo Hospital", 20},
                { "Chennai Central", 0},
                {"Fortis Malar Hospital", 15},
                {"Alwarpet", 22},
                {"Anna Nagar", 28},
                {"Besant Nagar", 30},
                {"Chrompet", 40},
                {"Egmore", 5},
                {"Guindy", 25},
                {"Kilpauk", 10},
                {"Medavakkam", 35},
                {"Mylapore", 15},
                {"Nanganallur", 45},
                {"Perambur", 30},
                {"T Nagar", 22},
                {"Tambaram", 50},
                {"Velachery", 30}
            }
            
        },
        {"Madras Medical Mission Hospital", 
            {
                {"Apollo Hospital", 18},
                {"Mogappair", 0},
                {"Fortis Malar Hospital", 16},
                {"Government General Hospital", 28},
                {"Alwarpet", 20},
                {"Anna Nagar", 5},
                {"Besant Nagar", 35},
                {"Chrompet", 22},
                {"Egmore", 25},
                {"Guindy", 18},
                {"Kilpauk", 20},
                {"Medavakkam", 40},
                {"Mylapore", 18},
                {"Nanganallur", 32},
                {"Perambur", 25},
                {"T Nagar", 20},
                {"Tambaram", 45},
                {"Velachery", 28}
            }
            
        },
        {"Sri Ramachandra Medical Center", 
            {
                {"Porur", 0},
                {"Apollo Hospital", 22},
                {"Fortis Malar Hospital", 20},
                {"Government General Hospital", 35},
                {"Madras Medical Mission Hospital", 30},
                {"Alwarpet", 25},
                {"Anna Nagar", 15},
                {"Besant Nagar", 28},
                {"Chrompet", 32},
                {"Egmore", 22},
                {"Guindy", 25},
                {"Kilpauk", 18},
                {"Medavakkam", 40},
                {"Mylapore",28},
                {"Nanganallur", 38},
                {"Perambur",30},
                {"T Nagar",25},
                {"Tambaram", 45},
                {"Velachery", 35}
            }
        }
    };
    
    // get symptom IDs from user input
    vector<int> symptom_ids;
    int num_symptoms;
    string ch;
    for (const auto& neighbor : hospitals) 
    {
        for (const auto& distTo : neighbor.dist) 
        {
            hospital.add_edge(neighbor.name,distTo.first,distTo.second);
        }}
       do
        {
            cout<<"Welcome to Health care chatbot!!"<<endl;
            cout<<"Hi, This is Aarogyam Chatbot to keep you healthy."<<endl;
            
            cout<<"\nI know that you approched me since you are suffering from some common symptoms:"<<endl;
            cout<<"1. Fever\n2. Cough\n3. Shortness of breath\n4. Chest pain\n5. Headache\n6. Nausea\n7. Dizziness\n8. Skin Rash\n9. Joints Pain\n10. Vomiting\n11. Fatigue\n12. Weight Loss\n13. Burning Micturition\n14. Itching\n15. Stomach Pain\n16. Restlessness\n"<<endl;
           
            cout << "How many symptoms do you have from the symptoms mentioned above?"<<endl;
            cin >> num_symptoms;
            cout << "Enter the IDs of your symptoms: "<<endl;
            for (int i = 0; i < num_symptoms; i++) 
            {
                int symptom_id;
                cin >> symptom_id;
                symptom_ids.push_back(symptom_id);
            }
            int condition_index = findMostLikelyCondition(graph, symptoms, conditions, symptom_ids);
          
            if (condition_index == -1) 
            {
                cout << "\nI cannot help you in finding your illness" << endl;
            } 
            else 
            {
                cout << "You most likely have " << conditions[condition_index].name << "." << endl;
                cout<<"risk scale: 1-10"<<endl;
                if(conditions[condition_index].risk_score<5)
                {
                    cout<<"Your risk score is "<<conditions[condition_index].risk_score<<endl<<"Its not serious."<<endl;
                    cout<<"\nI suggest you to consult a doctor online to get to Medication "<<endl;
                    cout<<"\nDo you want to consult a doctor online?(Yes or No)"<<endl;
                    cin>>ch;
                    if(ch =="Yes" || ch=="yes")
                    {
                        string disease= conditions[condition_index].name;
                        if(disease=="Common cold")
                        {
                            Ram.timeSlots("Ram",disease,BookedPatientsData);
                            
                        }
                        else if(disease=="Flu")
                        {
                            cout<<"hi";
                            Raj.timeSlots("Raj",disease,BookedPatientsData);
                            
                        }
                        else if(disease=="Migraine")
                        {
                            Sathya.timeSlots("Sathya",disease,BookedPatientsData);
                        }
                        
                    }
                    else
                    {
                        cout<<"Okay, It is Small Problem. So, You can take rest. You will be cure..."<<endl;
                        
                        
                    }
                    
                }
                else
                {
                    cout<<"\nYour risk score is "<<conditions[condition_index].risk_score<<endl<<"Its serious.You should go to a hospital"<<endl;
                    cout<<"\nEnter your locality name to recommend you the closest hospital near your area?"<<endl;
                    cin.ignore();
                    getline(cin,inputarea);
                }   
                inputarea[0] = toupper(inputarea[0]);   
                string s1=inputarea;
                int found=s1.find(" ");
                if(found){
                    inputarea[found+1] = toupper(inputarea[found+1]); 
                   
                    cout<<inputarea;
                }
                dijkstra(hospital, inputarea);
            }
           
        cout<<"\nThank You for contacting AAROKIYAM CHATBOT\n\n"<<endl;
        cout<<"\n Do you want to continue to chat with chatbot? Yes/No"<<endl;
        cin>>ch1;
        }while(ch1=="Yes" || ch1=="yes");
        
    
    
    return 0;
}
