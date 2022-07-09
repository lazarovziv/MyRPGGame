using RpgAPI.Enums;
using System.ComponentModel.DataAnnotations;

namespace RpgAPI.Models
{
    public class NPCEnemy
    {
        [Key]
        public int EnemyId { get; set; }
        [Required]
        public int BattleTimeout { get; set; }
        [Required]
        public float WanderAreaRadius { get; set; }
        [Required]
        public float BattleAreaRadius { get; set; }
        [Required]
        public EnemyType EnemyType { get; set; }

        //Foreign Key to Game Entity
        public ICollection<GameEntity>? GameEntity { get; set; }
    }
}
