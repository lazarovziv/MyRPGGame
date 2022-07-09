﻿using Microsoft.EntityFrameworkCore;
using RpgAPI.Data_Access;
using RpgAPI.Models;

namespace RpgAPI.Repository
{
    public class GameEntitiesRepo : IGameEntitiesRepo
    {
        private readonly RPGGameDBContext _context;

        public GameEntitiesRepo(RPGGameDBContext DbContext)
        {
            _context = DbContext;
        }

        public bool UpdateGameEntity(GameEntity entity)
        {
            if (entity == null || entity.Player == null || entity.GameMap == null)
                return false;

            _context.GameEntities.Update(entity);
            _context.Players.Update(entity.Player);
            _context.GameMaps.Update(entity.GameMap);

            //_context.GameEntities.Remove(entity);
            //_context.Players.Remove(entity.Player);
            //_context.GameMaps.Remove(entity.GameMap);

            //var gameEntity = new GameEntity
            //{
            //    EntityId = entity.EntityId,
            //    Name = entity.Name,
            //    Level = entity.Level,
            //    Gender = entity.Gender,
            //    CurrentHealthPoints = entity.CurrentHealthPoints,
            //    MaxHealthPoints = entity.MaxHealthPoints,
            //    AttackPoints = entity.AttackPoints,
            //    DefencePoints = entity.DefencePoints,
            //    Speed = entity.Speed,
            //    PositionX = entity.PositionX,
            //    PositionY = entity.PositionY,
            //    GameMapId = entity.GameMapId,
            //    PlayerId = entity.PlayerId,
            //};

            //var player = new Player
            //{
            //    PlayerId = entity.PlayerId,
            //    StrengthPoints = entity.Player.StrengthPoints,
            //    IntelligencePoints = entity.Player.IntelligencePoints,
            //    CriticalPoints = entity.Player.CriticalPoints,
            //    ExpPoints = entity.Player.ExpPoints
            //};

            //var gameMap = new GameMap
            //{
            //    GameMapId = entity.GameMapId,
            //    WorldMapRow = entity.GameMap.WorldMapRow,
            //    WorldMapCol = entity.GameMap.WorldMapCol
            //};

            //await _context.GameEntities.AddAsync(gameEntity);
            //await _context.Players.AddAsync(player);
            //await _context.GameMaps.AddAsync(gameMap);

            return true;
        }

        public void DeleteGameEntity(GameEntity entity)
        {
            if (entity == null || entity.Player == null || entity.GameMap == null)
                throw new ArgumentNullException(nameof(entity));

            _context.GameEntities.Remove(entity);
            _context.Players.Remove(entity.Player);
            _context.GameMaps.Remove(entity.GameMap);

            //    var gameEntity = new GameEntity
            //    {
            //        EntityId = entity.EntityId,
            //        Name = entity.Name,
            //        Level = entity.Level,
            //        Gender = entity.Gender,
            //        CurrentHealthPoints = entity.CurrentHealthPoints,
            //        MaxHealthPoints = entity.MaxHealthPoints,
            //        AttackPoints = entity.AttackPoints,
            //        DefencePoints = entity.DefencePoints,
            //        CurrentManaPoints = entity.CurrentManaPoints,
            //        MaxManaPoints = entity.MaxManaPoints,
            //        Speed = entity.Speed,
            //        PositionX = entity.PositionX,
            //        PositionY = entity.PositionY,
            //        GameMapId = entity.GameMapId,
            //        PlayerId = entity.PlayerId
            //    };

            //    var player = new Player
            //    {
            //        PlayerId = entity.PlayerId,
            //        StrengthPoints = entity.Player.StrengthPoints,
            //        IntelligencePoints = entity.Player.IntelligencePoints,
            //        CriticalPoints = entity.Player.CriticalPoints,
            //        ExpPoints = entity.Player.ExpPoints
            //    };

            //    var gameMap = new GameMap
            //    {
            //        GameMapId = entity.GameMapId,
            //        WorldMapRow = entity.GameMap.WorldMapRow,
            //        WorldMapCol = entity.GameMap.WorldMapCol
            //    };

            //_context.GameEntities.Remove(gameEntity);
            //_context.Players.Remove(player);
            //_context.GameMaps.Remove(gameMap);
        }

        public async Task CreateGameEntity(GameEntity entity)
        {
            if (entity == null || entity.GameMap == null)
                throw new ArgumentNullException(nameof(entity));

            else if (
                        (entity.IsPlayer == true && entity.Player == null)
                        ||
                        (entity.IsPlayer == false && entity.Enemy == null)
                    )
                throw new ArgumentNullException(nameof(entity));


            await _context.GameEntities.AddAsync(entity);
            await _context.GameMaps.AddAsync(entity.GameMap);

            if (entity.IsPlayer == true)
                await _context.Players.AddAsync(entity.Player);
            else
                await _context.Enemies.AddAsync(entity.Enemy);

            //    var gameEntity = new GameEntity
            //    {
            //        EntityId = entity.EntityId,
            //        Name = entity.Name,
            //        Level = entity.Level,
            //        Gender = entity.Gender,
            //        CurrentHealthPoints = entity.CurrentHealthPoints,
            //        MaxHealthPoints = entity.MaxHealthPoints,
            //        AttackPoints = entity.AttackPoints,
            //        DefencePoints = entity.DefencePoints,
            //        CurrentManaPoints = entity.CurrentManaPoints,
            //        MaxManaPoints = entity.MaxManaPoints,
            //        Speed = entity.Speed,
            //        PositionX = entity.PositionX,
            //        PositionY = entity.PositionY,
            //        GameMapId = entity.GameMapId,
            //        PlayerId = entity.PlayerId
            //    };

            //    var player = new Player
            //    {
            //        PlayerId = entity.PlayerId,
            //        StrengthPoints = entity.Player.StrengthPoints,
            //        IntelligencePoints = entity.Player.IntelligencePoints,
            //        CriticalPoints = entity.Player.CriticalPoints,
            //        ExpPoints = entity.Player.ExpPoints
            //    };

            //    var gameMap = new GameMap
            //    {
            //        GameMapId = entity.GameMapId,
            //        WorldMapRow = entity.GameMap.WorldMapRow,
            //        WorldMapCol = entity.GameMap.WorldMapCol
            //    };

            //    await _context.GameEntities.AddAsync(gameEntity);
            //    await _context.Players.AddAsync(player);
            //    await _context.GameMaps.AddAsync(gameMap);
        }

        public async Task SaveChanges()
        {
            await _context.SaveChangesAsync();
        }

        public async Task<IEnumerable<GameEntity>> GetAllGameEntities()
        {
            var query = from gameEntity in _context.GameEntities
                        from player in _context.Players
                        from gameMap in _context.GameMaps
                        from enemy in _context.Enemies
                        where gameEntity.PlayerId == player.PlayerId
                        && gameEntity.GameMapId == gameMap.GameMapId

                        select new GameEntity
                        {
                            EntityId = gameEntity.EntityId,
                            Name = gameEntity.Name,
                            Level = gameEntity.Level,
                            Gender = gameEntity.Gender,
                            CurrentHealthPoints = gameEntity.CurrentHealthPoints,
                            MaxHealthPoints = gameEntity.MaxHealthPoints,
                            AttackPoints = gameEntity.AttackPoints,
                            DefencePoints = gameEntity.DefencePoints,
                            CurrentManaPoints = gameEntity.CurrentManaPoints,
                            MaxManaPoints = gameEntity.MaxManaPoints,
                            Speed = gameEntity.Speed,
                            PositionX = gameEntity.PositionX,
                            PositionY = gameEntity.PositionY,
                            GameMapId = gameEntity.GameMapId,

                            GameMap = new GameMap
                            {
                                GameMapId = gameMap.GameMapId,
                                WorldMapRow = gameMap.WorldMapRow,
                                WorldMapCol = gameMap.WorldMapCol
                            },

                            Player = new Player
                            {
                                PlayerId = player.PlayerId,
                                StrengthPoints = player.StrengthPoints,
                                IntelligencePoints = player.IntelligencePoints,
                                CriticalPoints = player.CriticalPoints,
                                ExpPoints = player.ExpPoints,
                                PlayerType = player.PlayerType
                            },
                            Enemy = new NPCEnemy
                            {
                                EnemyId = enemy.EnemyId,
                                BattleTimeout = enemy.BattleTimeout,
                                BattleAreaRadius = enemy.BattleAreaRadius,
                                WanderAreaRadius = enemy.WanderAreaRadius,
                                EnemyType = enemy.EnemyType
                            }
                        };

            return await query.ToListAsync();
        }

        public async Task<IEnumerable<GameEntity>> GetAllPlayersEntities()
        {
            var query = from gameEntity in _context.GameEntities
                        from player in _context.Players
                        from gameMap in _context.GameMaps
                        from enemy in _context.Enemies
                        where gameEntity.PlayerId == player.PlayerId
                        && gameEntity.GameMapId == gameMap.GameMapId
                        && gameEntity.IsPlayer == true

                        select new GameEntity
                        {
                            EntityId = gameEntity.EntityId,
                            Name = gameEntity.Name,
                            Level = gameEntity.Level,
                            Gender = gameEntity.Gender,
                            CurrentHealthPoints = gameEntity.CurrentHealthPoints,
                            MaxHealthPoints = gameEntity.MaxHealthPoints,
                            AttackPoints = gameEntity.AttackPoints,
                            DefencePoints = gameEntity.DefencePoints,
                            CurrentManaPoints = gameEntity.CurrentManaPoints,
                            MaxManaPoints = gameEntity.MaxManaPoints,
                            Speed = gameEntity.Speed,
                            PositionX = gameEntity.PositionX,
                            PositionY = gameEntity.PositionY,
                            GameMapId = gameEntity.GameMapId,

                            GameMap = new GameMap
                            {
                                GameMapId = gameMap.GameMapId,
                                WorldMapRow = gameMap.WorldMapRow,
                                WorldMapCol = gameMap.WorldMapCol
                            },

                            Player = new Player
                            {
                                PlayerId = player.PlayerId,
                                StrengthPoints = player.StrengthPoints,
                                IntelligencePoints = player.IntelligencePoints,
                                CriticalPoints = player.CriticalPoints,
                                ExpPoints = player.ExpPoints,
                                PlayerType = player.PlayerType
                            }
                        };

            return await query.ToListAsync();
        }

        public async Task<IEnumerable<GameEntity>> GetAllNPCEnemies()
        {
            var query = from gameEntity in _context.GameEntities
                        from gameMap in _context.GameMaps
                        from enemy in _context.Enemies
                        where gameEntity.NpcEnemyId == enemy.EnemyId
                        && gameEntity.GameMapId == gameMap.GameMapId
                        && gameEntity.IsPlayer == false
                        select new GameEntity
                        {
                            EntityId = gameEntity.EntityId,
                            Name = gameEntity.Name,
                            Level = gameEntity.Level,
                            Gender = gameEntity.Gender,
                            CurrentHealthPoints = gameEntity.CurrentHealthPoints,
                            MaxHealthPoints = gameEntity.MaxHealthPoints,
                            AttackPoints = gameEntity.AttackPoints,
                            DefencePoints = gameEntity.DefencePoints,
                            CurrentManaPoints = gameEntity.CurrentManaPoints,
                            MaxManaPoints = gameEntity.MaxManaPoints,
                            Speed = gameEntity.Speed,
                            PositionX = gameEntity.PositionX,
                            PositionY = gameEntity.PositionY,
                            GameMapId = gameEntity.GameMapId,
                            GameMap = new GameMap
                            {
                                GameMapId = gameMap.GameMapId,
                                WorldMapRow = gameMap.WorldMapRow,
                                WorldMapCol = gameMap.WorldMapCol
                            },
                            Enemy = new NPCEnemy
                            {
                                EnemyId = enemy.EnemyId,
                                BattleTimeout = enemy.BattleTimeout,
                                BattleAreaRadius = enemy.BattleAreaRadius,
                                WanderAreaRadius = enemy.WanderAreaRadius,
                                EnemyType = enemy.EnemyType
                            }
                        };

            return await query.ToListAsync();
        }

        public async Task<GameEntity> GetGameEntityById(long id)
        {
            var query = from gameEntity in _context.GameEntities
                        from player in _context.Players
                        from gameMap in _context.GameMaps
                        from enemy in _context.Enemies

                        where gameEntity.EntityId == id
                        && gameEntity.PlayerId == player.PlayerId
                        && gameEntity.GameMapId == gameMap.GameMapId

                        select new GameEntity
                        {
                            EntityId = gameEntity.EntityId,
                            Name = gameEntity.Name,
                            Level = gameEntity.Level,
                            Gender = gameEntity.Gender,
                            CurrentHealthPoints = gameEntity.CurrentHealthPoints,
                            MaxHealthPoints = gameEntity.MaxHealthPoints,
                            AttackPoints = gameEntity.AttackPoints,
                            DefencePoints = gameEntity.DefencePoints,
                            CurrentManaPoints = gameEntity.CurrentManaPoints,
                            MaxManaPoints = gameEntity.MaxManaPoints,
                            Speed = gameEntity.Speed,
                            PositionX = gameEntity.PositionX,
                            PositionY = gameEntity.PositionY,
                            GameMapId = gameEntity.GameMapId,

                            GameMap = new GameMap
                            {
                                GameMapId = gameMap.GameMapId,
                                WorldMapRow = gameMap.WorldMapRow,
                                WorldMapCol = gameMap.WorldMapCol
                            },

                            Player = new Player
                            {
                                PlayerId = player.PlayerId,
                                StrengthPoints = player.StrengthPoints,
                                IntelligencePoints = player.IntelligencePoints,
                                CriticalPoints = player.CriticalPoints,
                                ExpPoints = player.ExpPoints,
                                PlayerType = player.PlayerType
                            },
                            Enemy = new NPCEnemy
                            {
                                EnemyId = enemy.EnemyId,
                                BattleTimeout = enemy.BattleTimeout,
                                BattleAreaRadius = enemy.BattleAreaRadius,
                                WanderAreaRadius = enemy.WanderAreaRadius,
                                EnemyType = enemy.EnemyType
                            }
                        };

            return await query.FirstOrDefaultAsync();
        }

        public async Task<GameEntity> GetPlayerEntityById(int id)
        {
            var query = from gameEntity in _context.GameEntities
                        from player in _context.Players
                        from gameMap in _context.GameMaps

                        where gameEntity.EntityId == id
                        && gameEntity.PlayerId == player.PlayerId
                        && gameEntity.GameMapId == gameMap.GameMapId
                        && gameEntity.IsPlayer == true

                        select new GameEntity
                        {
                            EntityId = gameEntity.EntityId,
                            Name = gameEntity.Name,
                            Level = gameEntity.Level,
                            Gender = gameEntity.Gender,
                            CurrentHealthPoints = gameEntity.CurrentHealthPoints,
                            MaxHealthPoints = gameEntity.MaxHealthPoints,
                            AttackPoints = gameEntity.AttackPoints,
                            DefencePoints = gameEntity.DefencePoints,
                            CurrentManaPoints = gameEntity.CurrentManaPoints,
                            MaxManaPoints = gameEntity.MaxManaPoints,
                            Speed = gameEntity.Speed,
                            PositionX = gameEntity.PositionX,
                            PositionY = gameEntity.PositionY,
                            GameMapId = gameEntity.GameMapId,

                            GameMap = new GameMap
                            {
                                GameMapId = gameMap.GameMapId,
                                WorldMapRow = gameMap.WorldMapRow,
                                WorldMapCol = gameMap.WorldMapCol
                            },

                            Player = new Player
                            {
                                PlayerId = player.PlayerId,
                                StrengthPoints = player.StrengthPoints,
                                IntelligencePoints = player.IntelligencePoints,
                                CriticalPoints = player.CriticalPoints,
                                ExpPoints = player.ExpPoints,
                                PlayerType = player.PlayerType
                            }
                        };

            return await query.FirstOrDefaultAsync();
        }

        public async Task<GameEntity> GetEnemyEntityById(int id)
        {
            var query = from gameEntity in _context.GameEntities
                        from gameMap in _context.GameMaps
                        from enemy in _context.Enemies

                        where gameEntity.EntityId == id
                        && gameEntity.GameMapId == gameMap.GameMapId
                        && gameEntity.IsPlayer == false

                        select new GameEntity
                        {
                            EntityId = gameEntity.EntityId,
                            Name = gameEntity.Name,
                            Level = gameEntity.Level,
                            Gender = gameEntity.Gender,
                            CurrentHealthPoints = gameEntity.CurrentHealthPoints,
                            MaxHealthPoints = gameEntity.MaxHealthPoints,
                            AttackPoints = gameEntity.AttackPoints,
                            DefencePoints = gameEntity.DefencePoints,
                            CurrentManaPoints = gameEntity.CurrentManaPoints,
                            MaxManaPoints = gameEntity.MaxManaPoints,
                            Speed = gameEntity.Speed,
                            PositionX = gameEntity.PositionX,
                            PositionY = gameEntity.PositionY,
                            GameMapId = gameEntity.GameMapId,

                            GameMap = new GameMap
                            {
                                GameMapId = gameMap.GameMapId,
                                WorldMapRow = gameMap.WorldMapRow,
                                WorldMapCol = gameMap.WorldMapCol
                            },
                            Enemy = new NPCEnemy
                            {
                                EnemyId = enemy.EnemyId,
                                BattleTimeout = enemy.BattleTimeout,
                                BattleAreaRadius = enemy.BattleAreaRadius,
                                WanderAreaRadius = enemy.WanderAreaRadius,
                                EnemyType = enemy.EnemyType
                            }
                        };

            return await query.FirstOrDefaultAsync();
        }
    }
}



//public async Task<IEnumerable<GameEntity>> GetAllGameEntities()
//{
//    var entities = await _context.GameEntities.ToListAsync();

//    return entities;
//}


//public async Task<GameEntity> GetGameEntityById(long id)
//{
//    var entity = await _context.GameEntities.FirstOrDefaultAsync(e => e.EntityId == id);

//    return entity;
//}


//public void DeleteGameEntity(GameEntity entity)
//{
//    if (entity == null)
//        throw new ArgumentNullException(nameof(entity));

//    _context.GameEntities.Remove(entity);
//}


//public async Task CreateGameEntity(GameEntity entity)
//{
//    if (entity == null)
//        throw new ArgumentNullException(nameof(entity));

//    await _context.GameEntities.AddAsync(entity);
//}