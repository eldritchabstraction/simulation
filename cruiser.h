/*
A Cruiser is a Ship with moderate speed, firepower, and resistance.
When told to attack it will start firing at the target if it is in range.
At each update, it will stop attacking if the target is either no longer afloat
(i.e. is sinking or sunk), or is out of range. As long as the target is both afloat
and in range, it will keep firing at it.

Initial values:
fuel capacity and initial amount: 1000, maximum speed 20., fuel consumption 10.tons/nm, 
resistance 6, firepower 3, maximum attacking range 15
*/

/* 
This skeleton file shows the required public and protected interface for the class, which you may not modify. 
If no protected members are shown, there must be none in your version. 
If any protected or private members are shown here, then your class must also have them and use them as intended.
You should delete this comment.
*/

public:
	// initialize, then output constructor message
	Cruiser(const std::string& name_, Point position_);
	// output destructor message
	~Cruiser();

	// perform Cruiser-specific behavior
	void update() override;

	void describe() const override;

	// respond to an attack
    void receive_hit(int hit_force, Ship* attacker_ptr) override;

	// start an attack on a target ship
	void attack(Ship* target_ptr_) override;

	void stop_attack() override;


