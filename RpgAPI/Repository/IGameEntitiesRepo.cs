using RpgAPI.Models;

namespace RpgAPI.Repository
{
    public interface IGameEntitiesRepo
    {
        Task SaveChanges();
        Task<IEnumerable<GameEntity>> GetAllGameEntities();
        Task<GameEntity> GetGameEntityById(long id);
        Task CreateGameEntity(GameEntity entity);
        void DeleteGameEntity(GameEntity entity);
        bool UpdateGameEntity(GameEntity entity);
    }
}
