using RpgAPI.Dtos.CreateDtos;
using RpgAPI.Models;

namespace RpgAPI.EntitiesValidation
{
    public static class GameEntitiesValidation
    {
        //public static bool ValidateEnemiesGet(IEnumerable<GameEntity>? enemies)
        //{
        //    if (ValidateNullEntity(enemies) == false)
        //        return false;

        //    foreach (var enemy in enemies)
        //    {
        //        if (ValidateNullEntity(enemy) == false)
        //            return false;
        //    }

        //    return true;
        //}

        public static bool ValidateGameEntities<T>(IEnumerable<T> enemies)
        {
            if (ValidateNullEntity(enemies) == false)
                return false;

            foreach (var enemy in enemies)
            {
                if (ValidateNullEntity(enemy) == false)
                    return false;
            }

            return true;
        }

        public static bool ValidateNullEntity<T>(T entity)
        {
            return entity != null;
        }

        //public static void ValidateNullEntityGet(long? id)
        //{
        //    var error = new ArgumentNullException(nameof(id));

        //    if (id == null)
        //        throw error;
        //}

        public static bool ValidateNullEntityID<T>(T id)
        {
            return id != null;
        }

        public static void ValidateNullEntityCreate(GameEntityCreateDto? entity)
        {
            var error = new ArgumentNullException(nameof(entity));

            if (entity == null || entity.GameMap == null)
                throw error;

            else if (entity.Enemy == null && entity.Player == null)
                throw error;
        }

        public static bool ValidateNullEntityUpdate(GameEntity? entity)
        {
            if (entity == null || entity.GameMap == null)
                return false;

            else if (entity.Enemy == null && entity.Player == null)
                return false;

            return true;
        }

        public static void ValidateNullEntityDelete(long? entityId)
        {
            var error = new ArgumentNullException(nameof(entityId));

            if (entityId == null)
                throw error;
        }

        
    }
}
