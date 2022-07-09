CREATE TABLE [dbo].[GameEntity]
(
	[Id] BIGINT NOT NULL PRIMARY KEY, 
    [Name] NCHAR(10) NOT NULL, 
    [Level] INT NOT NULL, 
    [Gender] INT NOT NULL, 
    [CurrentHealthPoints] INT NOT NULL, 
    [MaxHealthPoints] INT NOT NULL, 
    [AttackPoints] INT NOT NULL, 
    [DefencePoints] INT NOT NULL, 
    [Speed] FLOAT NOT NULL, 
    [PositionX] FLOAT NOT NULL, 
    [PositionY] FLOAT NOT NULL, 
    [PlayerId] INT NOT NULL, 
    [GameMapId] INT NOT NULL,
    FOREIGN KEY (PlayerId) REFERENCES Player(Id),
    FOREIGN KEY (GameMapId) REFERENCES GameMap(Id)
)
