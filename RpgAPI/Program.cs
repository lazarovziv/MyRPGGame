using AutoMapper;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Data.SqlClient;
using Microsoft.EntityFrameworkCore;
using RpgAPI.Data_Access;
using RpgAPI.Dtos.ReadDtos;
using RpgAPI.Repository;
using RpgAPI.Dtos.CreateDtos;
using RpgAPI.Models;
using RpgAPI.EntitiesValidation;

namespace RpgAPI;

public class Program
{
    public static void Main(string[] args)
    {
        var builder = WebApplication.CreateBuilder(args);

        // Add services to the container.
        builder.Services.AddAuthorization();

        //Get the Connection string from the config file
        var sqlConBuilder = new SqlConnectionStringBuilder
        {
            ConnectionString = builder.Configuration.GetConnectionString("default")
        };

        //Connect to SQL Server with our Connection String
        builder.Services.AddDbContext<RPGGameDBContext>(opt => opt.UseSqlServer(sqlConBuilder.ConnectionString, 
            sqlopt => sqlopt.EnableRetryOnFailure(2)));

        builder.Services.AddScoped<IGameEntitiesRepo, GameEntitiesRepo>();

        builder.Services.AddAutoMapper(AppDomain.CurrentDomain.GetAssemblies());

        var app = builder.Build();

        // Configure the HTTP request pipeline.

        app.UseHttpsRedirection();

        app.UseAuthorization();


        //API End Points


        //GET Requests

        //Getting all of the Enemies Entities
        app.MapGet("api/get/enemies", async (IGameEntitiesRepo repo, IMapper mapper) =>
        {
            var enemies = await repo.GetAllNPCEnemies();

            //if (enemies == null)
            //    return Results.NotFound(new ArgumentNullException(nameof(enemies)));

            //if(GameEntitiesValidation.ValidateEnemiesGet(enemies) == Results.NotFound())
            //    return Results.NotFound(new ArgumentNullException(nameof(enemies)));

            //return Results.Ok(mapper.Map<IEnumerable<GameEntityReadDto>>(enemies));

            return GameEntitiesValidation.ValidateGameEntities(enemies) == true ?
                    Results.Ok(mapper.Map<IEnumerable<GameEntityReadDto>>(enemies)) 
                    : 
                    Results.NotFound(new ArgumentNullException(nameof(enemies)));
        });


        //Getting a Player Entity by Id
        app.MapGet("api/get/players/{playerId}", async (IGameEntitiesRepo repo, IMapper mapper, [FromRoute] int? playerId) =>
        {
            //GameEntitiesValidation.ValidateNullEntityGet(playerId);

            if(GameEntitiesValidation.ValidateNullEntityID(playerId) == false)
                return Results.NotFound(new ArgumentNullException(nameof(playerId)));

            var entity = await repo.GetPlayerEntityById(playerId);

            //if(entity == null)
            //    return Results.NotFound(new ArgumentNullException(nameof(entity)));

            //if(GameEntitiesValidation.ValidateNullEntity(entity) == false)
            //    return Results.NotFound(new ArgumentNullException(nameof(entity)));

            //return Results.Ok(mapper.Map<GameEntityReadDto>(entity));

            return GameEntitiesValidation.ValidateNullEntity(entity) == true ?
                    Results.Ok(mapper.Map<GameEntityReadDto>(entity)) 
                    :
                    Results.NotFound(new ArgumentNullException(nameof(entity)));
        });


        //Getting a Enemy by Id
        app.MapGet("api/get/enemies/{enemyId}", async (IGameEntitiesRepo repo, IMapper mapper, [FromRoute] int? enemyId) =>
        {
            //GameEntitiesValidation.ValidateNullEntityGet(enemyId);

            if(GameEntitiesValidation.ValidateNullEntityID(enemyId) == false)
                return Results.NotFound(new ArgumentNullException(nameof(enemyId)));

            var enemy = await repo.GetEnemyEntityById(enemyId);

            //if (enemy == null)
            //    return Results.NotFound(new ArgumentNullException(nameof(enemy)));

            //return Results.Ok(mapper.Map<GameEntityReadDto>(enemy));

            return GameEntitiesValidation.ValidateNullEntity(enemy) == true ?
                    Results.Ok(mapper.Map<GameEntityReadDto>(enemy))
                    :
                    Results.NotFound(new ArgumentNullException(nameof(enemy)));
        });




        //POST Requests

        //Creating an array of Enemies Entities
        app.MapPost("api/create/enemies", async (IGameEntitiesRepo repo, IMapper mapper, [FromBody] GameEntityCreateDto?[] entities) =>
        {
            for(int i = 0; i < entities.Length; i++)
            {
                if (entities[i] == null)
                    return Results.BadRequest(new ArgumentNullException(nameof(entities)));

                GameEntitiesValidation.ValidateNullEntityCreate(entities[i]);

                var entity = mapper.Map<GameEntity>(entities[i]);

                await repo.CreateGameEntity(entity);
            }

            await repo.SaveChanges();

            return Results.NoContent();
        });

        //Creating a Game Entity
        app.MapPost("api/create/gameentity", async (IGameEntitiesRepo repo, IMapper mapper, [FromBody] GameEntityCreateDto? jsonEntity) =>
        {
            if (jsonEntity == null)
                return Results.BadRequest(new ArgumentNullException(nameof(jsonEntity)));

            GameEntitiesValidation.ValidateNullEntityCreate(jsonEntity);

            var entity = mapper.Map<GameEntity>(jsonEntity);

            await repo.CreateGameEntity(entity);

            await repo.SaveChanges();

            return Results.NoContent();
        });



        //PUT Requests

        //Update a Game Entity
        app.MapPut("api/update/entities/{entityId}", async (IGameEntitiesRepo repo, [FromRoute] int? entityId, 
            [FromBody] GameEntity updatedEntity) =>
        {
            var error = new ArgumentOutOfRangeException(nameof(updatedEntity));

            //check for User Input Validation
            if (entityId == null)
                Results.BadRequest();

            else if (GameEntitiesValidation.ValidateNullEntityUpdate(updatedEntity) == false)
                return Results.NotFound(error);

            var updatePlayer = updatedEntity.IsPlayer;

            GameEntity previousEntity = updatePlayer == true ? await repo.GetPlayerEntityById(entityId) : await repo.GetEnemyEntityById(entityId);

            //Check if we got back from our DB an Empty Object
            if (GameEntitiesValidation.ValidateNullEntityUpdate(previousEntity) == false)
                return Results.NotFound(error);

            //If the Entity Id of the prev and the updated Objects are not Equal
            if (previousEntity.EntityId != updatedEntity.EntityId)
                return Results.NotFound();

            //If the Player Id of the prev and the updated Objects are not Equal
            else if (updatePlayer == true)
            {
                if (previousEntity.PlayerId != updatedEntity.PlayerId)
                    return Results.NotFound();
            }

            //If the Enemy Id of the prev and the updated Objects are not Equal
            else
            {
                if (previousEntity.EnemyId != updatedEntity.EnemyId)
                    return Results.NotFound();
            }

            var updateResult = repo.UpdateGameEntity(updatedEntity);

            if (updateResult == false)
                return Results.BadRequest(error);

            await repo.SaveChanges();

            return Results.NoContent();
        });



        //DELETE Requests

        //Delete a Player Entity
        app.MapDelete("api/delete/players/{playerId}", async (IGameEntitiesRepo repo, IMapper mapper, [FromRoute] int? playerId) =>
        {
            GameEntitiesValidation.ValidateNullEntityDelete(playerId);

            var player = await repo.GetPlayerEntityById(playerId);

            if (player == null)
                return Results.NotFound(new ArgumentOutOfRangeException(nameof(player)));

            repo.DeleteGameEntity(player);

            await repo.SaveChanges();

            return Results.NoContent();
        });

        //Delete a Enemy Entity
        app.MapDelete("api/delete/enemies/{enemyId}", async (IGameEntitiesRepo repo, [FromRoute] int? enemyId) =>
        {
            GameEntitiesValidation.ValidateNullEntityDelete(enemyId);

            var enemy = await repo.GetEnemyEntityById(enemyId);

            if (enemy == null)
                return Results.NotFound(new ArgumentOutOfRangeException(nameof(enemy)));

            repo.DeleteGameEntity(enemy);

            await repo.SaveChanges();

            return Results.NoContent();
        });


        app.Run();
    }
}