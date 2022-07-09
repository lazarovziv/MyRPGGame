using RpgAPI.Enums;
using System.ComponentModel.DataAnnotations;

namespace RpgAPI.Dtos.CreateDtos
{
    public class PlayerCreateDto
    {
        [Required]
        public int PlayerId { get; set; }
        [Required]
        public int StrengthPoints { get; set; }
        [Required]
        public int IntelligencePoints { get; set; }
        [Required]
        public int CriticalPoints { get; set; }
        [Required]
        public int ExpPoints { get; set; }
        [Required]
        public PlayerType PlayerType { get; set; }
    }
}