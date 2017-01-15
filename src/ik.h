#ifndef __IK_H__
#define __IK_H__

class vec3 {
private:
	float x,y,z;
public:
	vec3(float newX, float newY, float newZ);
	float magnitude() const;
	vec3 unit() const;
	vec3 operator+(const vec3& other) const;
	vec3 operator-(const vec3& other) const;
	vec3 operator-() const;
	vec3 operator*(float scalar) const;
	float getX() { return x; }
	float getY() { return y; }
	float getZ() { return z; }
};

void initRobot(float robotArmLength, float firstArmHeight);
void moveTo(vec3 pos);
void moveTo(float x, float y, float z);
void moveAlongLine(vec3 to);
void moveAlongCircle(vec3 center, float radius);
void moveAlongX(vec3 center, float radius);
void drawBoard();

#endif