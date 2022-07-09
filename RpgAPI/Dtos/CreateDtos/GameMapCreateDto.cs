using System.ComponentModel.DataAnnotations;

namespace RpgAPI.Dtos.CreateDtos
{
    public class GameMapCreateDto
    {
        [Required]
        public int GameMapId { get; set; }
        [Required]
        public int WorldMapRow { get; set; }
        [Required]
        public int WorldMapCol { get; set; }
    }
}