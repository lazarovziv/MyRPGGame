using RpgAPI.Models;

namespace RpgAPI.Repository
{
    public interface IGameEntitiesRepo
    {
        Task CreateGameEntity(GameEntity entity);
        void DeleteGameEntity(GameEntity entity);
        Task<IEnumerable<GameEntity>> GetAllNPCEnemies();
        Task<GameEntity> GetEnemyEntityById(int? id);
        Task<GameEntity> GetPlayerEntityById(long? id);
        Task SaveChanges();
        bool UpdateGameEntity(GameEntity entity);
    }
}