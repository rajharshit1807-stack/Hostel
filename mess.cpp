#include <iostream>
#include <string>
using namespace std;

const int MAX_STUDENTS = 100;
const int DAYS = 7;

// Base class for common person details
class Person
{
protected:
    string id;
    string name;

public:
    void setDetails(string studentID, string studentName)
    {
        id = studentID;
        name = studentName;
    }

    string getID() const
    {
        return id;
    }

    string getName() const
    {
        return name;
    }
};

// Student inherits ID and name from Person
class Student : public Person
{
private:
    bool breakfast[DAYS];
    bool dinner[DAYS];

public:
    Student()
    {
        id = "";
        name = "";

        for (int i = 0; i < DAYS; i++)
        {
            breakfast[i] = false;
            dinner[i] = false;
        }
    }

    void setStudent(string studentID, string studentName)
    {
        setDetails(studentID, studentName);
    }

    void setBreakfast(int day, bool value)
    {
        breakfast[day] = value;
    }

    void setDinner(int day, bool value)
    {
        dinner[day] = value;
    }

    bool wantsBreakfast(int day) const
    {
        return breakfast[day];
    }

    bool wantsDinner(int day) const
    {
        return dinner[day];
    }

    void displayStudent() const
    {
        cout << "\nStudent ID: " << id;
        cout << "\nName: " << name;

        cout << "\nBreakfast days: ";
        bool found = false;
        for (int i = 0; i < DAYS; i++)
        {
            if (breakfast[i])
            {
                cout << i + 1 << " ";
                found = true;
            }
        }
        if (!found)
            cout << "None";

        cout << "\nDinner days: ";
        found = false;
        for (int i = 0; i < DAYS; i++)
        {
            if (dinner[i])
            {
                cout << i + 1 << " ";
                found = true;
            }
        }
        if (!found)
            cout << "None";

        cout << "\n--------------------------------\n";
    }
};

class Menu
{
private:
    string day, breakfast, lunch, snacks, dinner;

public:
    void setMenu(string d, string b, string l, string s, string dn)
    {
        day = d;
        breakfast = b;
        lunch = l;
        snacks = s;
        dinner = dn;
    }

    void displayMenu() const
    {
        cout << "\n========== " << day << " MENU ==========\n";
        cout << "Breakfast: " << breakfast << endl;
        cout << "Lunch    : " << lunch << endl;
        cout << "Snacks   : " << snacks << endl;
        cout << "Dinner   : " << dinner << endl;
    }
};

class Mess
{
private:
    Menu weeklyMenu[DAYS];
    Student students[MAX_STUDENTS];
    int studentCount;

    int findStudent(string studentID) const
    {
        for (int i = 0; i < studentCount; i++)
        {
            if (students[i].getID() == studentID)
                return i;
        }
        return -1;
    }

    void selectDay(int &day)
    {
        cout << "\nSelect day:\n";
        cout << "1. Monday\n2. Tuesday\n3. Wednesday\n4. Thursday\n";
        cout << "5. Friday\n6. Saturday\n7. Sunday\n";
        cout << "Enter day number: ";
        cin >> day;

        if (day < 1 || day > 7)
        {
            cout << "Invalid day!\n";
            day = -1;
        }
        else
        {
            day--;
        }
    }

public:
    Mess()
    {
        studentCount = 0;

        weeklyMenu[0].setMenu("Monday", "Aloo Paratha", "Rajma Rice", "Samosa", "Roti, Dal and Paneer");
        weeklyMenu[1].setMenu("Tuesday", "Poha and Tea", "Chole Chawal", "Bread Pakora", "Mix Veg, Roti and Dal");
        weeklyMenu[2].setMenu("Wednesday", "Idli Sambhar", "Kadhi Chawal", "Veg Sandwich", "Aloo Gobi, Roti and Rice");
        weeklyMenu[3].setMenu("Thursday", "Puri Sabzi", "Dal Tadka, Roti and Rice", "Biscuits and Tea", "Paneer, Roti and Dal");
        weeklyMenu[4].setMenu("Friday", "Upma and Banana", "Veg Pulao and Raita", "Pav Bhaji", "Dal Makhani, Roti and Rice");
        weeklyMenu[5].setMenu("Saturday", "Chole Bhature", "Paneer Butter Masala and Roti", "Maggi", "Veg Biryani and Raita");
        weeklyMenu[6].setMenu("Sunday", "Dosa and Chutney", "Special Thali", "Pasta", "Matar Paneer, Roti and Kheer");
    }

    void registerStudent()
    {
        if (studentCount >= MAX_STUDENTS)
        {
            cout << "Student limit reached!\n";
            return;
        }

        string studentID, studentName;
        cout << "Enter student ID: ";
        cin >> studentID;

        if (findStudent(studentID) != -1)
        {
            cout << "This student ID is already registered.\n";
            return;
        }

        cout << "Enter student name: ";
        cin.ignore();
        getline(cin, studentName);

        students[studentCount].setStudent(studentID, studentName);
        studentCount++;

        cout << "Student registered successfully.\n";
    }

    void viewMenu()
    {
        int day;
        selectDay(day);
        if (day >= 0)
            weeklyMenu[day].displayMenu();
    }

    void viewWeeklyMenu()
    {
        for (int i = 0; i < DAYS; i++)
            weeklyMenu[i].displayMenu();
    }

    void registerMeal()
    {
        string studentID;
        cout << "Enter your student ID: ";
        cin >> studentID;

        int index = findStudent(studentID);
        if (index == -1)
        {
            cout << "Student not found. Please register first.\n";
            return;
        }

        int day;
        selectDay(day);
        if (day < 0)
            return;

        char choice;
        cout << "Do you want breakfast? (Y/N): ";
        cin >> choice;

        if (choice == 'Y' || choice == 'y')
            students[index].setBreakfast(day, true);
        else if (choice == 'N' || choice == 'n')
            students[index].setBreakfast(day, false);
        else
        {
            cout << "Invalid choice.\n";
            return;
        }

        cout << "Do you want dinner? (Y/N): ";
        cin >> choice;

        if (choice == 'Y' || choice == 'y')
            students[index].setDinner(day, true);
        else if (choice == 'N' || choice == 'n')
            students[index].setDinner(day, false);
        else
        {
            cout << "Invalid choice.\n";
            return;
        }

        cout << "Meal preferences saved.\n";
    }

    void showMealCount()
    {
        int day;
        selectDay(day);
        if (day < 0)
            return;

        int breakfastCount = 0;
        int dinnerCount = 0;

        for (int i = 0; i < studentCount; i++)
        {
            if (students[i].wantsBreakfast(day))
                breakfastCount++;
            if (students[i].wantsDinner(day))
                dinnerCount++;
        }

        cout << "\nBreakfast required: " << breakfastCount << " students\n";
        cout << "Dinner required: " << dinnerCount << " students\n";
    }

    void showStudents()
    {
        if (studentCount == 0)
        {
            cout << "No students registered yet.\n";
            return;
        }

        for (int i = 0; i < studentCount; i++)
            students[i].displayStudent();
    }

    void searchStudent()
    {
        string studentID;
        cout << "Enter student ID to search: ";
        cin >> studentID;

        int index = findStudent(studentID);
        if (index == -1)
            cout << "Student record not found.\n";
        else
            students[index].displayStudent();
    }
};

int main()
{
    Mess mess;
    int mainChoice;

    do
    {
        cout << "\n===== SMART MESS MANAGEMENT SYSTEM =====\n";
        cout << "1. Student Menu\n2. Mess Admin Menu\n3. Exit\n";
        cout << "Enter choice: ";
        cin >> mainChoice;

        if (mainChoice == 1)
        {
            int choice;
            do
            {
                cout << "\n===== STUDENT MENU =====\n";
                cout << "1. Register student\n2. View daily menu\n";
                cout << "3. Register/update meals\n4. Back\n";
                cout << "Enter choice: ";
                cin >> choice;

                if (choice == 1)
                    mess.registerStudent();
                else if (choice == 2)
                    mess.viewMenu();
                else if (choice == 3)
                    mess.registerMeal();
                else if (choice != 4)
                    cout << "Invalid choice.\n";

            } while (choice != 4);
        }
        else if (mainChoice == 2)
        {
            int choice;
            do
            {
                cout << "\n===== MESS ADMIN MENU =====\n";
                cout << "1. View weekly menu\n2. View meal count\n";
                cout << "3. View registered students\n4. Search student record\n5. Back\n";
                cout << "Enter choice: ";
                cin >> choice;

                if (choice == 1)
                    mess.viewWeeklyMenu();
                else if (choice == 2)
                    mess.showMealCount();
                else if (choice == 3)
                    mess.showStudents();
                else if (choice == 4)
                    mess.searchStudent();
                else if (choice != 5)
                    cout << "Invalid choice.\n";

            } while (choice != 5);
        }
        else if (mainChoice == 3)
        {
            cout << "Thank you for using the system!\n";
        }
        else
        {
            cout << "Invalid choice.\n";
        }

    } while (mainChoice != 3);

    return 0;
}
