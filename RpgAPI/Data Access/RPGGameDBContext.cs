using Microsoft.EntityFrameworkCore;
using RpgAPI.Models;

namespace RpgAPI.Data_Access
{
    public class RPGGameDBContext : DbContext
    {
        public RPGGameDBContext(DbContextOptions<RPGGameDBContext> options) : base(options)
        {

        }

        public DbSet<GameEntity> GameEntities => Set<GameEntity>();
        public DbSet<GameMap> GameMaps => Set<GameMap>();
        public DbSet<Player> Players => Set<Player>();
    }
}
