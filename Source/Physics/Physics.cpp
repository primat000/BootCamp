#include "Physics.h"
#include "EventSystem.h"

RigidBody::RigidBody() {}

RigidBody::RigidBody(sf::Vector2f c, sf::Vector2f s, float r, float m)
{
  _coords = c;
  _speed  = s;
  _radius = r;
  _mass   = m;
}


void RigidBody::Update(float t)
{
  _coords.x += _speed.x * t;
  _coords.y += _speed.y * t;
}

void RigidBody::SetRadius(float r) { _radius = r; }

void RigidBody::SetCoordinates(sf::Vector2f c) { _coords = c; }

void RigidBody::SetX(float arg) { _coords.x = arg; }

void RigidBody::SetY(float arg) { _coords.y = arg; }

void RigidBody::SetSpeed(sf::Vector2f s) { _speed = s; }

float RigidBody::GetRadius() const { return _radius; }

float RigidBody::GetX() const { return _coords.x; }

float RigidBody::GetY() const { return _coords.y; }

sf::Vector2f RigidBody::GetCoordinates() const { return _coords; }

float RigidBody::GetMass() const { return _mass; }
float RigidBody::GetSpeedX() const { return _speed.x; }
float RigidBody::GetSpeedY() const { return _speed.y; }
sf::Vector2f RigidBody::GetSpeed() const { return _speed; }


void RigidBody::SetSpeedX(float arg) { _speed.x = arg; }
void RigidBody::SetSpeedY(float arg) { _speed.y = arg; }
void RigidBody::SetMass(float arg) { _mass = arg; }

constexpr float tolerance = 1.04f;

void ResolveCollision(RigidBody & go1, RigidBody & go2)
{

  float dx = (go1.GetX() + go1.GetRadius() - go2.GetX() - go2.GetRadius());
  float dy = (go1.GetY() + go1.GetRadius() - go2.GetY() - go2.GetRadius());

  float d1x = go1.GetX() + go1.GetRadius();
  float d1y = go1.GetY() + go1.GetRadius();

  float d2x = go2.GetX() + go2.GetRadius();
  float d2y = go2.GetY() + go2.GetRadius();

  float dist = sqrt(dx * dx + dy * dy);

  float overlap = tolerance * (dist - go1.GetRadius() - go2.GetRadius());

  go1.SetX(go1.GetX() - (overlap * (go1.GetX() - go2.GetX()) / dist));
  go1.SetY(go1.GetY() - (overlap * (go1.GetY() - go2.GetY()) / dist));

  go2.SetX(go2.GetX() + (overlap * (go1.GetX() - go2.GetX()) / dist));
  go2.SetY(go2.GetY() + (overlap * (go1.GetY() - go2.GetY()) / dist));

  //normal
  float nx = (d1x - d2x) / dist;
  float ny = (d1y - d2y) / dist;
  //Tanget
  float tx     = -ny;
  float ty     = nx;
  float dpTan1 = go1.GetSpeedX() * tx + go1.GetSpeedY() * ty;
  float dpTan2 = go2.GetSpeedX() * tx + go2.GetSpeedX() * ty;

  float dpNorm1 = go1.GetSpeedX() * nx + go1.GetSpeedY() * ny;
  float dpNorm2 = go2.GetSpeedX() * nx + go2.GetSpeedX() * ny;

  float m1 =
    (dpNorm1 * (go1.GetMass() - go2.GetMass()) + 2.0f * go2.GetMass() * dpNorm2) / (go1.GetMass() + go2.GetMass());
  float m2 =
    (dpNorm2 * (go2.GetMass() - go1.GetMass()) + 2.0f * go1.GetMass() * dpNorm1) / (go1.GetMass() + go2.GetMass());

  // Update ball velocities
  go1.SetSpeedX((tx * dpTan1 + nx * m1) * 0.9);
  go1.SetSpeedY((ty * dpTan1 + ny * m1) * 0.9);
  go2.SetSpeedX((tx * dpTan2 + nx * m2) * 0.9);
  go2.SetSpeedY((ty * dpTan2 + ny * m2) * 0.9);

}

bool Collided(RigidBody go1, RigidBody go2)
{
  float dx       = (go1.GetX() + go1.GetRadius() - go2.GetX() - go2.GetRadius());
  float dy       = (go1.GetY() + go1.GetRadius() - go2.GetY() - go2.GetRadius());
  float distance = dx * dx + dy * dy;
  return (distance < (go1.GetRadius() + go2.GetRadius()) * (go1.GetRadius() + go2.GetRadius()));
}

void RandomFill(RigidBody * RigidBodysFunc, int length)
{
  std::srand(static_cast<unsigned int>(std::time(nullptr)));

  for(int i = 0; i < length; ++i)
  {
    RigidBodysFunc[i].SetRadius(10.f + std::rand() / ((RAND_MAX + 1u) / 100));
    RigidBodysFunc[i].SetCoordinates({static_cast<float>(0 + std::rand() / ((RAND_MAX + 1u) / WindowResolution::_W)),
                                      static_cast<float>(0 + std::rand() / ((RAND_MAX + 1u) / WindowResolution::_H))});
    RigidBodysFunc[i].SetSpeed({static_cast<float>(0 + std::rand() / ((RAND_MAX + 1u) / 200)),
                                static_cast<float>(0 + std::rand() / ((RAND_MAX + 1u) / 15))});
    RigidBodysFunc[i].SetMass(10.f + std::rand() / ((RAND_MAX + 1u) / 20));
  }
}

void RigidBody::SetColliderVisible(bool param)
{
	_isColliderVisible = param;
}

bool RigidBody::IsColliderVisible()
{
	return _isColliderVisible;
}
