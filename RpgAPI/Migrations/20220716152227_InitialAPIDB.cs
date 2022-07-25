using Microsoft.EntityFrameworkCore.Migrations;

#nullable disable

namespace RpgAPI.Migrations
{
    public partial class InitialAPIDB : Migration
    {
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.CreateTable(
                name: "Enemies",
                columns: table => new
                {
                    EnemyId = table.Column<int>(type: "int", nullable: false),
                    BattleTimeout = table.Column<int>(type: "int", nullable: false),
                    WanderAreaRadius = table.Column<float>(type: "real", nullable: false),
                    BattleAreaRadius = table.Column<float>(type: "real", nullable: false),
                    EnemyType = table.Column<int>(type: "int", nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Enemies", x => x.EnemyId);
                });

            migrationBuilder.CreateTable(
                name: "GameMaps",
                columns: table => new
                {
                    GameMapId = table.Column<int>(type: "int", nullable: false),
                    WorldMapRow = table.Column<int>(type: "int", nullable: false),
                    WorldMapCol = table.Column<int>(type: "int", nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_GameMaps", x => x.GameMapId);
                });

            migrationBuilder.CreateTable(
                name: "Players",
                columns: table => new
                {
                    PlayerId = table.Column<int>(type: "int", nullable: false),
                    StrengthPoints = table.Column<int>(type: "int", nullable: false),
                    IntelligencePoints = table.Column<int>(type: "int", nullable: false),
                    CriticalPoints = table.Column<int>(type: "int", nullable: false),
                    ExpPoints = table.Column<int>(type: "int", nullable: false),
                    PlayerType = table.Column<int>(type: "int", nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Players", x => x.PlayerId);
                });

            migrationBuilder.CreateTable(
                name: "GameEntities",
                columns: table => new
                {
                    EntityId = table.Column<long>(type: "bigint", nullable: false),
                    Name = table.Column<string>(type: "nvarchar(10)", maxLength: 10, nullable: false),
                    Level = table.Column<int>(type: "int", nullable: false),
                    Gender = table.Column<int>(type: "int", nullable: false),
                    CurrentHealthPoints = table.Column<int>(type: "int", nullable: false),
                    MaxHealthPoints = table.Column<int>(type: "int", nullable: false),
                    AttackPoints = table.Column<int>(type: "int", nullable: false),
                    DefencePoints = table.Column<int>(type: "int", nullable: false),
                    CurrentManaPoints = table.Column<int>(type: "int", nullable: false),
                    MaxManaPoints = table.Column<int>(type: "int", nullable: false),
                    Speed = table.Column<float>(type: "real", nullable: false),
                    PositionX = table.Column<float>(type: "real", nullable: false),
                    PositionY = table.Column<float>(type: "real", nullable: false),
                    IsPlayer = table.Column<bool>(type: "bit", nullable: false),
                    GameMapId = table.Column<int>(type: "int", nullable: false),
                    PlayerId = table.Column<int>(type: "int", nullable: true),
                    EnemyId = table.Column<int>(type: "int", nullable: true)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_GameEntities", x => x.EntityId);
                    table.ForeignKey(
                        name: "FK_GameEntities_Enemies_EnemyId",
                        column: x => x.EnemyId,
                        principalTable: "Enemies",
                        principalColumn: "EnemyId");
                    table.ForeignKey(
                        name: "FK_GameEntities_GameMaps_GameMapId",
                        column: x => x.GameMapId,
                        principalTable: "GameMaps",
                        principalColumn: "GameMapId",
                        onDelete: ReferentialAction.Cascade);
                    table.ForeignKey(
                        name: "FK_GameEntities_Players_PlayerId",
                        column: x => x.PlayerId,
                        principalTable: "Players",
                        principalColumn: "PlayerId");
                });

            migrationBuilder.InsertData(
                table: "Enemies",
                columns: new[] { "EnemyId", "BattleAreaRadius", "BattleTimeout", "EnemyType", "WanderAreaRadius" },
                values: new object[,]
                {
                    { 123, 250f, 60, 0, 400f },
                    { 456, 32f, 60, 1, 5000f }
                });

            migrationBuilder.InsertData(
                table: "GameMaps",
                columns: new[] { "GameMapId", "WorldMapCol", "WorldMapRow" },
                values: new object[,]
                {
                    { 1, 72, 45 },
                    { 2, 22, 50 },
                    { 3, 12, 42 },
                    { 4, 90, 56 }
                });

            migrationBuilder.InsertData(
                table: "Players",
                columns: new[] { "PlayerId", "CriticalPoints", "ExpPoints", "IntelligencePoints", "PlayerType", "StrengthPoints" },
                values: new object[,]
                {
                    { 1, 300, 500, 200, 0, 100 },
                    { 2, 400, 500, 300, 1, 200 }
                });

            migrationBuilder.InsertData(
                table: "GameEntities",
                columns: new[] { "EntityId", "AttackPoints", "CurrentHealthPoints", "CurrentManaPoints", "DefencePoints", "EnemyId", "GameMapId", "Gender", "IsPlayer", "Level", "MaxHealthPoints", "MaxManaPoints", "Name", "PlayerId", "PositionX", "PositionY", "Speed" },
                values: new object[,]
                {
                    { 11111111L, 34, 200, 322, 600, 456, 3, 1, false, 3, 400, 800, "Enemy2", null, 44f, 55f, 1f },
                    { 12345678L, 300, 100, 500, 400, null, 1, 1, true, 1, 200, 600, "Test", 1, 43f, 98f, 3f },
                    { 22222223L, 60, 500, 76, 34, null, 4, 1, true, 50, 1000, 200, "Player 2", 2, 22f, 76f, 6f },
                    { 87654321L, 500, 100, 400, 600, 123, 2, 2, false, 45, 2000, 1000, "Enemy1", null, 56f, 21f, 5f }
                });

            migrationBuilder.CreateIndex(
                name: "IX_GameEntities_EnemyId",
                table: "GameEntities",
                column: "EnemyId");

            migrationBuilder.CreateIndex(
                name: "IX_GameEntities_GameMapId",
                table: "GameEntities",
                column: "GameMapId");

            migrationBuilder.CreateIndex(
                name: "IX_GameEntities_PlayerId",
                table: "GameEntities",
                column: "PlayerId");
        }

        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropTable(
                name: "GameEntities");

            migrationBuilder.DropTable(
                name: "Enemies");

            migrationBuilder.DropTable(
                name: "GameMaps");

            migrationBuilder.DropTable(
                name: "Players");
        }
    }
}
