using RpgAPI.Enums;
using System.ComponentModel.DataAnnotations;

namespace RpgAPI.Dtos.CreateDtos;

public class NPCEnemyCreateDto
{
    [Required]
    public int EnemyId { get; set; }
    [Required]
    public int BattleTimeout { get; set; }
    [Required]
    public float WanderAreaRadius { get; set; }
    [Required]
    public float BattleAreaRadius { get; set; }
    [Required]
    public EnemyType EnemyType { get; set; }
}