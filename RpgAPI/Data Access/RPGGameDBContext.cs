using Microsoft.EntityFrameworkCore;
using RpgAPI.Models;

namespace RpgAPI.Data_Access
{
    public class RPGGameDBContext : DbContext
    {
        public DbSet<GameEntity> GameEntities => Set<GameEntity>();
        public DbSet<GameMap> GameMaps => Set<GameMap>();
        public DbSet<Player> Players => Set<Player>();
        public DbSet<NPCEnemy> Enemies => Set<NPCEnemy>();

        public RPGGameDBContext(DbContextOptions<RPGGameDBContext> options) : base(options)
        {

        }

        protected override void OnModelCreating(ModelBuilder builder)
        {
            builder.Entity<GameEntity>().ToTable("GameEntities");
            builder.Entity<GameMap>().ToTable("GameMaps");
            builder.Entity<Player>().ToTable("Players");
            builder.Entity<NPCEnemy>().ToTable("Enemies");

            builder.Entity<Player>().Property(p => p.PlayerType).HasConversion<int>();
            builder.Entity<NPCEnemy>().Property(e => e.EnemyType).HasConversion<int>();

            base.OnModelCreating(builder);
        }
    }
}
