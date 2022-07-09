using System.ComponentModel.DataAnnotations;

namespace RpgAPI.Dtos.CreateDtos
{
    public class GameEntityCreateDto
    {
        [Required]
        public long EntityId { get; set; }
        [Required]
        public string? Name { get; set; }
        [Required]
        public int Level { get; set; }
        [Required]
        public int Gender { get; set; }
        [Required]
        public int CurrentHealthPoints { get; set; }
        [Required]
        public int MaxHealthPoints { get; set; }
        [Required]
        public int AttackPoints { get; set; }
        [Required]
        public int DefencePoints { get; set; }
        [Required]
        public int CurrentManaPoints { get; set; }
        [Required]
        public int MaxManaPoints { get; set; }
        [Required]
        public float Speed { get; set; }
        [Required]
        public float PositionX { get; set; }
        [Required]
        public float PositionY { get; set; }
        [Required]
        public bool IsPlayer { get; set; }
        [Required]
        public int GameMapId { get; set; }
        public GameMapCreateDto? GameMap { get; set; }
        public int? PlayerId { get; set; }
        public PlayerCreateDto? Player { get; set; }
        public int? NpcEnemyId { get; set; }
        public NPCEnemyCreateDto? Enemy { get; set; }

    }
}
