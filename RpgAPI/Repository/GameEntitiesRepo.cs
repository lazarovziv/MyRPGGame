using Microsoft.EntityFrameworkCore;
using RpgAPI.Data_Access;
using RpgAPI.Models;

namespace RpgAPI.Repository;

public class GameEntitiesRepo : IGameEntitiesRepo
{
    private readonly RPGGameDBContext _context;

    public GameEntitiesRepo(RPGGameDBContext DbContext)
    {
        _context = DbContext;
    }

    public bool UpdateGameEntity(GameEntity entity)
    {
        if (entity.IsPlayer == true)
        {
            _context.GameEntities.Update(entity);
            _context.GameMaps.Update(entity.GameMap);
            _context.Players.Update(entity.Player);
        }
        else
        {
            _context.GameEntities.Update(entity);
            _context.GameMaps.Update(entity.GameMap);
            _context.Enemies.Update(entity.Enemy);
        }

        return true;
    }

    public void DeleteGameEntity(GameEntity entity)
    {
        if (entity.IsPlayer == true)
        {
            _context.GameEntities.Remove(entity);
            _context.GameMaps.Remove(entity.GameMap);
            _context.Players.Remove(entity.Player);
        }
        else
        {
            _context.GameEntities.Remove(entity);
            _context.GameMaps.Remove(entity.GameMap);
            _context.Enemies.Remove(entity.Enemy);
        }
    }

    public async Task CreateGameEntity(GameEntity entity)
    {
        await _context.GameEntities.AddAsync(entity);
        await _context.GameMaps.AddAsync(entity.GameMap);

        if (entity.IsPlayer == true)
            await _context.Players.AddAsync(entity.Player);
        else
            await _context.Enemies.AddAsync(entity.Enemy);
    }


    public async Task<IEnumerable<GameEntity>> GetAllNPCEnemies()
    {
        var query = from entity in _context.GameEntities
                    join map in _context.GameMaps
                    on entity.GameMapId equals map.GameMapId
                    join enemy in _context.Enemies
                    on entity.EnemyId equals enemy.EnemyId

                    where entity.IsPlayer == false

                    select new GameEntity
                    {
                        EntityId = entity.EntityId,
                        Name = entity.Name,
                        Level = entity.Level,
                        Gender = entity.Gender,
                        CurrentHealthPoints = entity.CurrentHealthPoints,
                        MaxHealthPoints = entity.MaxHealthPoints,
                        AttackPoints = entity.AttackPoints,
                        DefencePoints = entity.DefencePoints,
                        CurrentManaPoints = entity.CurrentManaPoints,
                        MaxManaPoints = entity.MaxManaPoints,
                        Speed = entity.Speed,
                        PositionX = entity.PositionX,
                        PositionY = entity.PositionY,
                        IsPlayer = entity.IsPlayer,

                        GameMapId = entity.GameMapId,
                        GameMap = new GameMap
                        {
                            GameMapId = map.GameMapId,
                            WorldMapRow = map.WorldMapRow,
                            WorldMapCol = map.WorldMapCol
                        },

                        EnemyId = entity.EnemyId,
                        Enemy = new NPCEnemy
                        {
                            EnemyId = enemy.EnemyId,
                            BattleTimeout = enemy.BattleTimeout,
                            BattleAreaRadius = enemy.BattleAreaRadius,
                            WanderAreaRadius = enemy.WanderAreaRadius,
                            EnemyType = enemy.EnemyType
                        }
                    };

        var results = await query.ToListAsync();

        return results;
    }

    public async Task<GameEntity> GetPlayerEntityById(long? id)
    {
        var query = from entity in _context.GameEntities
                    join player in _context.Players
                    on entity.PlayerId equals player.PlayerId
                    join map in _context.GameMaps
                    on entity.GameMapId equals map.GameMapId

                    where entity.IsPlayer == true
                    && entity.PlayerId == id

                    select new GameEntity
                    {
                        EntityId = entity.EntityId,
                        Name = entity.Name,
                        Level = entity.Level,
                        Gender = entity.Gender,
                        CurrentHealthPoints = entity.CurrentHealthPoints,
                        MaxHealthPoints = entity.MaxHealthPoints,
                        AttackPoints = entity.AttackPoints,
                        DefencePoints = entity.DefencePoints,
                        CurrentManaPoints = entity.CurrentManaPoints,
                        MaxManaPoints = entity.MaxManaPoints,
                        Speed = entity.Speed,
                        PositionX = entity.PositionX,
                        PositionY = entity.PositionY,
                        IsPlayer = entity.IsPlayer,

                        GameMapId = entity.GameMapId,
                        GameMap = new GameMap
                        {
                            GameMapId = map.GameMapId,
                            WorldMapRow = map.WorldMapRow,
                            WorldMapCol = map.WorldMapCol
                        },

                        PlayerId = entity.PlayerId,
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

        var result = await query.FirstOrDefaultAsync();

        if (result == null)
            throw new ArgumentNullException(nameof(query));

        return result;
    }

    public async Task<GameEntity> GetEnemyEntityById(int? id)
    {
        var query = from entity in _context.GameEntities
                    join map in _context.GameMaps
                    on entity.GameMapId equals map.GameMapId
                    join enemy in _context.Enemies
                    on entity.EnemyId equals enemy.EnemyId

                    where entity.IsPlayer == false
                    && entity.EnemyId == id

                    select new GameEntity
                    {
                        EntityId = entity.EntityId,
                        Name = entity.Name,
                        Level = entity.Level,
                        Gender = entity.Gender,
                        CurrentHealthPoints = entity.CurrentHealthPoints,
                        MaxHealthPoints = entity.MaxHealthPoints,
                        AttackPoints = entity.AttackPoints,
                        DefencePoints = entity.DefencePoints,
                        CurrentManaPoints = entity.CurrentManaPoints,
                        MaxManaPoints = entity.MaxManaPoints,
                        Speed = entity.Speed,
                        PositionX = entity.PositionX,
                        PositionY = entity.PositionY,
                        IsPlayer = entity.IsPlayer,

                        GameMapId = entity.GameMapId,
                        GameMap = new GameMap
                        {
                            GameMapId = map.GameMapId,
                            WorldMapRow = map.WorldMapRow,
                            WorldMapCol = map.WorldMapCol
                        },

                        EnemyId = entity.EnemyId,
                        Enemy = new NPCEnemy
                        {
                            EnemyId = enemy.EnemyId,
                            BattleTimeout = enemy.BattleTimeout,
                            BattleAreaRadius = enemy.BattleAreaRadius,
                            WanderAreaRadius = enemy.WanderAreaRadius,
                            EnemyType = enemy.EnemyType
                        }
                    };

        var result = await query.FirstOrDefaultAsync();

        if (result == null)
            throw new ArgumentNullException(nameof(query));

        return result;
    }

    public async Task SaveChanges()
    {
        await _context.SaveChangesAsync();
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




//public async Task<bool> UpdateGameEntity(GameEntity entity)
//{
//    if (entity == null || entity.GameMap == null || entity.Player == null)
//        return false;

//    _context.GameEntities.Remove(entity);
//    _context.Players.Remove(entity.Player);
//    _context.GameMaps.Remove(entity.GameMap);

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
//        Speed = entity.Speed,
//        PositionX = entity.PositionX,
//        PositionY = entity.PositionY,
//        GameMapId = entity.GameMapId,
//        PlayerId = entity.PlayerId,
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

//    return true;
//}




//public void DeleteGameEntity(GameEntity entity)
//{
//    if (entity == null || entity.Player == null || entity.GameMap == null)
//        throw new ArgumentNullException(nameof(entity));

//    _context.GameEntities.Remove(entity);
//    _context.Players.Remove(entity.Player);
//    _context.GameMaps.Remove(entity.GameMap);

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

//    _context.GameEntities.Remove(gameEntity);
//    _context.Players.Remove(player);
//    _context.GameMaps.Remove(gameMap);
//}