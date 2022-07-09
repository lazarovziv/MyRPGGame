using RpgAPI.Enums;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace RpgAPI.Models
{
    public class GameEntity
    {
        [Key]
        public long EntityId { get; set; }
        [Required]
        [MaxLength(10)]
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


        //Foreign Keys

        [ForeignKey("GameMap")]
        public int GameMapId { get; set; }
        public GameMap? GameMap { get; set; }

        [ForeignKey("Player")]
        public int? PlayerId { get; set; }
        public Player? Player { get; set; }

        [ForeignKey("NPCEnemy")]
        public int? NpcEnemyId { get; set; }
        public NPCEnemy? Enemy { get; set; }
    }
}
