using System.ComponentModel.DataAnnotations;

namespace RpgAPI.Models
{
    public class GameMap
    {
        [Key]
        public int GameMapId { get; set; }
        [Required]
        public int WorldMapRow { get; set; }
        [Required]
        public int WorldMapCol { get; set; }

        //Foreign Key to Game Entity
        public ICollection<GameEntity>? GameEntity { get; set; }
    }
}
