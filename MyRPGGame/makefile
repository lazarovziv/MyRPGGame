# main.out : src/main.cpp src/Circle.cpp src/Game.cpp src/GameEntity.cpp src/GameEntityMovement.cpp src/GameMap.cpp src/LandscapeEntity.cpp src/NPCEnemy.cpp src/Player.cpp src/Point.cpp src/Projectile.cpp src/Weapon.cpp src/GameEntityBattle.cpp include/Constants.h
# 	g++ src/main.cpp src/Circle.cpp src/Game.cpp src/GameEntity.cpp src/GameEntityMovement.cpp src/GameMap.cpp src/LandscapeEntity.cpp src/NPCEnemy.cpp src/Player.cpp src/Point.cpp src/Projectile.cpp src/Weapon.cpp src/GameEntityBattle.cpp include/Constants.h -o main.out -lm -lsfml-graphics -lsfml-window -lsfml-system

point.o : src/Point.cpp include/Point.hpp
	g++ -std=c++11 -c src/Point.cpp -o point.o

circle.o : src/Circle.cpp include/Circle.hpp point.o
	g++ -std=c++11 -c src/Circle.cpp -o circle.o point.o

weapon.o : src/Weapon.cpp include/Weapon.hpp
	g++ -std=c++11 -c src/Weapon.cpp -o weapon.o

movementSubject.o : src/MovementSubject.cpp include/MovementSubject.hpp
	g++ -std=c++11 -c src/MovementSubject.cpp -o movementSubject.o

gameEntity.o : src/GameEntity.cpp include/GameEntity.hpp weapon.o circle.o
	g++ -std=c++11 -c src/GameEntity.cpp -o gameEntity.o weapon.o circle.o

player.o : src/Player.cpp include/Player.hpp gameEntity.o movementSubject.o
	g++ -std=c++11 -c src/Player.cpp -o player.o gameEntity.o movementSubject.o

npcEnemy.o : src/NPCEnemy.cpp include/NPCEnemy.hpp gameEntity.o
	g++ -std=c++11 -c src/NPCEnemy.cpp -o npcEnemy.o gameEntity.o

landscapeEntity.o : src/LandscapeEntity.cpp include/LandscapeEntity.hpp gameEntity.o
	g++ -std=c++11 -c src/LandscapeEntity.cpp -o landscapeEntity.o gameEntity.o

gameMap.o : src/GameMap.cpp include/GameMap.hpp npcEnemy.o landscapeEntity.o
	g++ -std=c++11 -c src/GameMap.cpp -o gameMap.o npcEnemy.o landscapeEntity.o

game.o : src/Game.cpp include/Game.hpp gameMap.o player.o
	g++ -std=c++11 -c src/Game.cpp -o game.o gameMap.o player.o

main.o : src/main.cpp game.o
	g++ -std=c++11 -c src/main.cpp -o main.o game.o

main.out : main.o
	g++ -std=c++11 -o main.out main.o