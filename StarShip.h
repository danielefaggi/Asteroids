
#if !defined(D6_STAROBJECTS_H)
#define D6_STAROBJECTS_H

class StarObject {

protected:

	Vertex position;
	Vertex speed;

	Vertex max_position;

public:
	virtual void Update();
	
};

class StarShip : public StarObject {

	
};

class Asteroid : public StatObject {

};

#endif // !defined(D6_STAROBJECTS_H)
