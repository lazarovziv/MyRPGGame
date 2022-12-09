# main.out : src/main.cpp src/Circle.cpp src/Game.cpp src/GameEntity.cpp src/GameEntityMovement.cpp src/GameMap.cpp src/LandscapeEntity.cpp src/NPCEnemy.cpp src/Player.cpp src/Point.cpp src/Projectile.cpp src/Weapon.cpp src/GameEntityBattle.cpp include/Constants.h
# 	g++ src/main.cpp src/Circle.cpp src/Game.cpp src/GameEntity.cpp src/GameEntityMovement.cpp src/GameMap.cpp src/LandscapeEntity.cpp src/NPCEnemy.cpp src/Player.cpp src/Point.cpp src/Projectile.cpp src/Weapon.cpp src/GameEntityBattle.cpp include/Constants.h -o main.out -lm -lsfml-graphics -lsfml-window -lsfml-system

circle.o : src/Circle.cpp include/Circle.hpp
	g++ -std=c++11 src/Circle.cpp -o circle.o

point.o : src/Point.cpp include/Point.hpp
	g++ src/Point.cpp -o point.o

weapon.o : src/Weapon.cpp include/Weapon.hpp
	g++ src/Weapon.cpp -o weapon.o

movementObserver.o : src/MovementObserver.cpp include/MovementObserver.hpp
	g++ src/MovementObserver.cpp -o movementObserver.o

movementSubject.o : src/MovementSubject.cpp include/MovementSubject.hpp
	g++ src/MovementSubject.cpp -o movementSubject.o

gameEntity.o : src/GameEntity.cpp include/GameEntity.hpp weapon.o circle.o
	g++ src/GameEntity.cpp -o gameEntity.o

player.o : src/Player.cpp include/Player.hpp gameEntity.o movementSubject.o
	g++ src/Player.cpp -o player.o

npcEnemy.o : src/NPCEnemy.cpp include/NPCEnemy.hpp gameEntity.o circle.o movementObserver.o
	g++ src/NPCEnemy.cpp -o npcEnemy.o

landscapeEntity.o : src/LandscapeEntity.cpp include/LandscapeEntity.hpp gameEntity.o
	g++ src/LandscapeEntity.cpp -o landscapeEntity.o

gameMap.o : src/GameMap.cpp include/GameMap.hpp npcEnemy.o circle.o landscapeEntity.o
	g++ src/LandscapeEntity.cpp -o landscapeEntity.o

game.o : src/Game.cpp include/Game.hpp gameMap.o player.o npcEnemy.o landscapeEntity.o
	g++ src/Game.cpp -o game.o

main.out : src/main.cpp game.o
	g++ src/main.cpp -o main.out