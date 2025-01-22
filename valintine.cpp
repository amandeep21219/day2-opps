
class ValentineBrick;

// User class 
class User {
private:
    string name;
    int userId;
    vector<shared_ptr<ValentineBrick>> ownedBricks;
    vector<shared_ptr<ValentineBrick>> dedicatedTo;

public:
    User(const string &name, int userId) : name(name), userId(userId) {}

    void ownBrick(shared_ptr<ValentineBrick> &brick) {
        ownedBricks.push_back(brick);
    }

    void dedicateBrick(shared_ptr<ValentineBrick> &brick);

    const vector<std::shared_ptr<ValentineBrick>> &getOwnedBricks()  {
        return ownedBricks;
    }

    vector<shared_ptr<ValentineBrick>> &getDedicatedBricks()  {
        return dedicatedTo;
    }

    string &getName() const {
        return name;
    }

    int getUserId() const {
        return userId;
    }
};

// ValentineBrick class 
class ValentineBrick {
private:
    int id;
    string caption;
    string message;
    shared_ptr<User> owner;
    vector<std::shared_ptr<User>> dedications;
    int flagCount = 0;
    bool isVisible = true;

public:
    ValentineBrick(int id, string &caption, &message, shared_ptr<User> &owner)
        : id(id), caption(caption), message(message), owner(owner) {}

    void addDedication(shared_ptr<User> &user) {
        dedications.push_back(user);
    }

    void flag() {
        flagCount++;
        if (flagCount > 10) {
            isVisible = false;
        }
    }

    bool isBrickVisible()  {
        return isVisible;
    }

    shared_ptr<User> &getOwner()  {
        return owner;
    }

    vector<std::shared_ptr<User>> &getDedications()  {
        return dedications;
    }

    string &getMessage() {
        return message;
    }

    string &getCaption()  {
        return caption;
    }

    int getId()  {
        return id;
    }
};

// Implement dedicateBrick 
void User::dedicateBrick(shared_ptr<ValentineBrick> &brick) {
    dedicatedTo.push_back(brick);
    brick->addDedication(std::make_shared<User>(*this));
}

// ValentineCityWall class
class ValentineCityWall {
private:
    const int MAX_BRICKS = 90;
    vector<shared_ptr<ValentineBrick>> bricks;
    int wallId;

public:
    ValentineCityWall(int wallId) : wallId(wallId) {}

    bool addBrick(const shared_ptr<ValentineBrick> &brick) {
        if (bricks.size() < MAX_BRICKS) {
            bricks.push_back(brick);
            return true;
        }
        return false;
    }

    int getWallId() const {
        return wallId;
    }

    vector<shared_ptr<ValentineBrick>> &getBricks()  {
        return bricks;
    }

    void printWallDetails() const {
        cout << "Wall ID: " << wallId << endl;
        for (const auto &brick : bricks) {
            cout << "Brick ID: " << brick->getId()
                      << ", Caption: " << brick->getCaption()
                      << ", Message: " << brick->getMessage()
                      << ", Owner: " << brick->getOwner()->getName() << endl;
        }
    }
};

// ValentineSystemManager class
class ValentineSystemManager {
private:
    vector<shared_ptr<User>> users;
    shared_ptr<ValentineCityWall> cityWall;

public:
    ValentineSystemManager(shared_ptr<ValentineCityWall> &cityWall) : cityWall(cityWall) {}

    void addUser(const shared_ptr<User> user) {
        users.push_back(user);
    }

    void addBrickToWall(shared_ptr<ValentineBrick> &brick) {
        if (!cityWall->addBrick(brick)) {
            cout << "Error: Wall is full, cannot add more bricks." << endl;
        }
    }

    void displayHottestUser() {
        auto hottest = std::max_element(users.begin(), users.end(),
                                        [](const shared_ptr<User> &u1, shared_ptr<User> &u2) {
                                            return u1->getDedicatedBricks().size() < u2->getDedicatedBricks().size();
                                        });

        if (hottest != users.end()) {
            cout << "Hottest User: " << (*hottest)->getName()
                      << " with " << (*hottest)->getDedicatedBricks().size() << " dedications." << endl;
        }
    }

    void displayIgnoredUsers() {
        for (const auto &user : users) {
            if (user->getDedicatedBricks().empty()) {
                cout << "Ignored User: " << user->getName() << endl;
            }
        }
    }

    void printWallDetails() const {
        cityWall->printWallDetails();
    }

    vector<shared_ptr<User>> &getUsers() const {
        return users;
    }
};

// Main function
int main() {
    // Setup system
    auto cityWall = std::make_shared<ValentineCityWall>(1);
    ValentineSystemManager systemManager(cityWall);

    // Create Users
    for (int i = 1; i <= 10; i++) {
        cout << "Enter name for user " << i << ": ";
        string name;
        getline(cin, name);
        auto user = make_shared<User>(name, i);
        systemManager.addUser(user);
    }

    // Create / add bricks to wall
    systemManager.addBrickToWall(make_shared<ValentineBrick>(1, "To My Valentine", "I love you!", systemManager.getUsers()[0]));
    systemManager.addBrickToWall(make_shared<ValentineBrick>(2, "For the Love of My Life", "Forever and always.", systemManager.getUsers()[1]));

    // User dedicate bricks
    systemManager.getUsers()[0]->dedicateBrick(cityWall->getBricks()[1]);
    systemManager.getUsers()[1]->dedicateBrick(cityWall->getBricks()[0]);

    // Display result
    systemManager.displayHottestUser();
    systemManager.displayIgnoredUsers();
    systemManager.printWallDetails();

    return 0;
}
