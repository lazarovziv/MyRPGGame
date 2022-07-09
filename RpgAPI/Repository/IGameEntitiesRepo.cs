using RpgAPI.Models;

namespace RpgAPI.Repository
{
    public interface IGameEntitiesRepo
    {
        Task CreateGameEntity(GameEntity entity);
        void DeleteGameEntity(GameEntity entity);
        Task<IEnumerable<GameEntity>> GetAllGameEntities();
        Task<IEnumerable<GameEntity>> GetAllNPCEnemies();
        Task<IEnumerable<GameEntity>> GetAllPlayersEntities();
        Task<GameEntity> GetEnemyEntityById(int id);
        Task<GameEntity> GetGameEntityById(long id);
        Task<GameEntity> GetPlayerEntityById(int id);
        Task SaveChanges();
        bool UpdateGameEntity(GameEntity entity);
    }
}