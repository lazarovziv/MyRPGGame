using Microsoft.EntityFrameworkCore;
using RpgAPI.Models;

namespace RpgAPI.Data_Access;

public class RPGGameDBContext : DbContext
{
    public DbSet<GameEntity> GameEntities => Set<GameEntity>();
    public DbSet<GameMap> GameMaps => Set<GameMap>();
    public DbSet<Player> Players => Set<Player>();
    public DbSet<NPCEnemy> Enemies => Set<NPCEnemy>();

    public RPGGameDBContext(DbContextOptions<RPGGameDBContext> options) : base(options) { }

    protected override void OnModelCreating(ModelBuilder builder)
    {
        builder.Entity<GameEntity>().ToTable("GameEntities");
        builder.Entity<GameMap>().ToTable("GameMaps");
        builder.Entity<Player>().ToTable("Players");
        builder.Entity<NPCEnemy>().ToTable("Enemies");

        builder.Entity<Player>().Property(p => p.PlayerType).HasConversion<int>();
        builder.Entity<NPCEnemy>().Property(e => e.EnemyType).HasConversion<int>();

        SeedData(builder);

        base.OnModelCreating(builder);
    }

    private static void SeedData(ModelBuilder builder)
    {
        builder.Entity<GameMap>().HasData(new GameMap
        {
            GameMapId = 1,
            WorldMapRow = 45,
            WorldMapCol = 72
        },
        new GameMap
        {
            GameMapId= 2,
            WorldMapRow = 50,
            WorldMapCol = 22
        },
        new GameMap
        {
            GameMapId = 3,
            WorldMapRow = 42,
            WorldMapCol = 12
        },
        new GameMap
        {
            GameMapId = 4,
            WorldMapRow = 56,
            WorldMapCol = 90
        });

        builder.Entity<Player>().HasData(new Player
        {
            PlayerId = 1,
            StrengthPoints = 100,
            IntelligencePoints = 200,
            CriticalPoints = 300,
            ExpPoints = 500,
            PlayerType = Enums.PlayerType.Knight
        },
        new Player
        {
            PlayerId= 2,
            StrengthPoints = 200,
            IntelligencePoints = 300,
            CriticalPoints = 400,
            ExpPoints = 500,
            PlayerType = Enums.PlayerType.Wizard
        });

        builder.Entity<NPCEnemy>().HasData(new NPCEnemy
        {
            EnemyId = 123,
            BattleTimeout = 60,
            WanderAreaRadius = 400,
            BattleAreaRadius = 250,
            EnemyType = Enums.EnemyType.Worm
        },
        new NPCEnemy
        {
            EnemyId = 456,
            BattleTimeout = 60,
            WanderAreaRadius = 5000,
            BattleAreaRadius = 32,
            EnemyType = Enums.EnemyType.Snake
        });

        builder.Entity<GameEntity>().HasData(new GameEntity
        {
            EntityId = 12345678,
            Name = "Test",
            Level = 1,
            Gender = 1,
            CurrentHealthPoints = 100,
            MaxHealthPoints = 200,
            AttackPoints = 300,
            DefencePoints = 400,
            CurrentManaPoints = 500,
            MaxManaPoints = 600,
            Speed = 3,
            PositionX = 43,
            PositionY = 98,
            IsPlayer = true,
            GameMapId = 1,
            PlayerId = 1
        },
        new GameEntity
        {
            EntityId = 87654321,
            Name = "Enemy1",
            Level = 45,
            Gender = 2,
            CurrentHealthPoints = 100,
            MaxHealthPoints = 2000,
            AttackPoints = 500,
            DefencePoints = 600,
            CurrentManaPoints = 400,
            MaxManaPoints = 1000,
            Speed = 5,
            PositionX = 56,
            PositionY = 21,
            IsPlayer = false,
            GameMapId = 2,
            EnemyId = 123
        },
        new GameEntity
        {
            EntityId = 11111111,
            Name = "Enemy2",
            Level = 3,
            Gender = 1,
            CurrentHealthPoints = 200,
            MaxHealthPoints = 400,
            AttackPoints = 34,
            DefencePoints = 600,
            CurrentManaPoints = 322,
            MaxManaPoints = 800,
            Speed = 1,
            PositionX = 44,
            PositionY = 55,
            IsPlayer = false,
            GameMapId = 3,
            EnemyId = 456
        },
        new GameEntity
        {
            EntityId = 22222223,
            Name = "Player 2",
            Level = 50,
            Gender = 1,
            CurrentHealthPoints = 500,
            MaxHealthPoints = 1000,
            AttackPoints = 60,
            DefencePoints = 34,
            CurrentManaPoints = 76,
            MaxManaPoints = 200,
            Speed = 6,
            PositionX = 22,
            PositionY = 76,
            IsPlayer = true,
            GameMapId = 4,
            PlayerId = 2
        });
    }
}
