package code.service;

import code.entity.User;
import org.springframework.web.multipart.MultipartFile;

import java.util.List;


public interface UserService {
    public List<User> readAll();

    public User readById(long id);

    public void delete(long id);

    List<User> parseJson(MultipartFile file);

    List<User> getPage(long limit);
}
