using RpgAPI.Enums;
using System.ComponentModel.DataAnnotations;

namespace RpgAPI.Models;

public class Player
{
    [Key]
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

    //Foreign Key to Game Entity
    public ICollection<GameEntity>? GameEntity { get; set; }
}
