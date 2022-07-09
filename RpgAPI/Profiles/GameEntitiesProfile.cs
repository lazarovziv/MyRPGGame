using AutoMapper;
using RpgAPI.Dtos.CreateDtos;
using RpgAPI.Dtos.ReadDtos;
using RpgAPI.Dtos.UpdateDtos;
using RpgAPI.Models;

namespace RpgAPI.Profiles
{
    public class GameEntitiesProfile : Profile
    {
        public GameEntitiesProfile()
        {
            //Source -> Target
            CreateMap<GameEntity, GameEntityReadDto>();
            CreateMap<Player, PlayerReadDto>();
            CreateMap<GameMap, GameMapReadDto>();
            CreateMap<NPCEnemy, NPCEnemyReadDto>();

            CreateMap<GameEntityCreateDto, GameEntity>();
            CreateMap<PlayerCreateDto, Player>();
            CreateMap<GameEntityCreateDto, GameMap>();
            CreateMap<NPCEnemyCreateDto, NPCEnemy>();

            CreateMap<GameEntityUpdateDto, GameEntity>();
            CreateMap<PlayerUpdateDto, Player>();
            CreateMap<GameMapUpdateDto, GameMap>();
            CreateMap<NPCEnemyUpdateDto, NPCEnemy>();
        }
    }
}
